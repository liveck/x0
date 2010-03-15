#include <x0/io/async_writer.hpp>
#include <x0/io/connection_sink.hpp>
#include <x0/connection.hpp>
#include <x0/types.hpp>
#include <ev++.h>
#include <memory>

namespace x0 {

class async_writer // {{{
{
private:
	std::shared_ptr<connection_sink> sink_;
	source_ptr source_;
	completion_handler_type handler_;
	std::size_t bytes_transferred_;

public:
	async_writer(std::shared_ptr<connection_sink> snk, const source_ptr& src, const completion_handler_type& handler) :
		sink_(snk),
		source_(src),
		handler_(handler),
		bytes_transferred_(0)
	{
	}

public:
	static void write(const std::shared_ptr<connection_sink>& snk, const source_ptr& src, const completion_handler_type& handler)
	{
		async_writer *writer = new async_writer(snk, src, handler);
		writer->write();
	}

private:
	void callback(connection *)
	{
		write();
	}

	void write()
	{
		ssize_t rv = sink_->pump(*source_);
		//printf("pump: %ld (%s)\n", rv, rv < 0 ? strerror(errno) : "");

		if (rv == 0)
		{
			// finished
			handler_(rv, bytes_transferred_);
			delete this;
		}
		else if (rv > 0)
		{
			// we wrote something (if not even all) -> call back when fully transmitted
			bytes_transferred_ += rv;
			sink_->connection()->on_ready(std::bind(&async_writer::callback, this, std::placeholders::_1), ev::WRITE);
		}
		else if (errno == EAGAIN || errno == EINTR)
		{
			// call back as soon as sink is ready for more writes
			sink_->connection()->on_ready(std::bind(&async_writer::callback, this, std::placeholders::_1), ev::WRITE);
		}
	}
}; // }}}

void async_write(connection *target, const source_ptr& src, const completion_handler_type& handler)
{
	async_write(std::make_shared<connection_sink>(target), src, handler);
}

void async_write(const std::shared_ptr<connection_sink>& snk, const source_ptr& src, const completion_handler_type& handler)
{
	async_writer::write(snk, src, handler);
}

} // namespace x0