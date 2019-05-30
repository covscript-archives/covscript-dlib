// DLib
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/image_transforms.h"
#include "dlib/image_io.h"
// CovScript
#include <covscript/dll.hpp>
#include <covscript/cni.hpp>

CNI_ROOT_NAMESPACE {
	// Image Type
	using image_t = std::shared_ptr<dlib::array2d<dlib::bgr_pixel>>;

	CNI_TYPE_EXT_V(image_type, image_t, image, cs::var::make<image_t>(std::make_shared<dlib::array2d<dlib::bgr_pixel>>()))
	{
		CNI_V(get_width, [](const image_t &img) {
			return img->nc();
		})

		CNI_V(get_height, [](const image_t &img) {
			return img->nr();
		})

		unsigned char *
		to_raw_data(const image_t &img) {
			// Process 4-byte alignment
			long row_size = img->nc() * 3;
			if (row_size % 4 != 0)
				row_size += 4 - row_size % 4;
			unsigned char *dat = new unsigned char[img->nr() * row_size];
			for (long y = 0; y < img->nr(); ++y) {
				for (long x = 0; x < img->nc(); ++x) {
					std::size_t idx = y * row_size + x * 3;
					dat[idx + 0] = (*img)[y][x].blue;
					dat[idx + 1] = (*img)[y][x].green;
					dat[idx + 2] = (*img)[y][x].red;
				}
			}
			return dat;
		}

		CNI(to_raw_data)
	}

// Image I/O
	CNI_V(load_image, [](image_t &img, const std::string &path)
	{
		dlib::load_image(*img, path);
	})

	CNI_V(save_bmp, [](const image_t &img, const std::string &path)
	{
		dlib::save_bmp(*img, path);
	})

	CNI_V(save_dng, [](const image_t &img, const std::string &path)
	{
		dlib::save_dng(*img, path);
	})

	CNI_V(save_jpeg, [](const image_t &img, const std::string &path)
	{
		dlib::save_jpeg(*img, path);
	})

	CNI_V(save_png, [](const image_t &img, const std::string &path)
	{
		dlib::save_png(*img, path);
	})

// Scaling and Rotating
	CNI_V(pyramid_up, [](image_t &img)
	{
		dlib::pyramid_up(*img);
	})

	CNI_V(resize_image, [](double scale, image_t &img)
	{
		dlib::resize_image(scale, *img);
	})

	CNI_V(rotate_image, [](const image_t &in, image_t &out, double angle)
	{
		dlib::rotate_image(*in, *out, angle);
	})

	void sub_image(const image_t &in, image_t &out, const dlib::rectangle &rect)
	{
		auto img = dlib::sub_image(*in, rect);
		dlib::assign_image(*out, img);
	}

	CNI(sub_image)

// Object Detection
	cs::var frontal_face_detect(const image_t &img)
	{
		static dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
		cs::var dat = cs::var::make<cs::array>();
		cs::array &arr = dat.val<cs::array>();
		std::vector<dlib::rectangle> dets = detector(*img);
		for (auto &it : dets)
			arr.emplace_back(it);
		return dat;
	}

	CNI(frontal_face_detect)
}

CNI_ENABLE_TYPE_EXT_V(image_type, cni_root_namespace::image_t, dlib::image)