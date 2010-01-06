#ifndef sw_x0_fileinfo_hpp
#define sw_x0_fileinfo_hpp (1)

#include <x0/api.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <sys/stat.h>
#include <string>
#include <map>

namespace x0 {

class fileinfo_service;
class plugin;

//! \addtogroup core
//@{

/** file info cache object.
 *
 * \see fileinfo_service, server
 */
class X0_API fileinfo :
	public boost::noncopyable
{
private:
	std::string filename_;

	bool exists_;
	struct stat st_;
	mutable std::string etag_;
	mutable std::string mtime_;
	mutable std::string mimetype_;
	std::map<const plugin *, void *> data_;

	friend class fileinfo_service;

public:
	explicit fileinfo(const std::string& filename);

	std::string filename() const;

	std::size_t size() const;
	time_t mtime() const;

	bool exists() const;
	bool is_directory() const;
	bool is_regular() const;
	bool is_executable() const;

	// custom-data
	void bind(const plugin *self, void *data);
	template<typename T> T& operator()(const plugin *self) const;
	template<typename T> T& get(const plugin *self) const;
	void unbind(const plugin *self);

	// HTTP related high-level properties
	std::string etag() const;
	std::string last_modified() const;
	std::string mimetype() const;

private:
	std::string get_mime_type(std::string ext) const;
};

//@}

} // namespace x0

#include <x0/fileinfo.ipp>

#endif
