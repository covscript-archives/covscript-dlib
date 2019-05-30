import imgui
import dlib
# Load Image
var img=new dlib.image
dlib.load_image(img, context.cmd_args()[1])
dlib.pyramid_up(img)
var dets=dlib.frontal_face_detect(img)
# Detect Face
if dets.empty()
    throw runtime.exception("No face detected")
end
var app=imgui.window_application(1100, 500, "Test")
var img_list=new array
foreach it in dets
    var face=new dlib.image
    dlib.sub_image(img, face, it)
    img_list.push_back(imgui.make_image(face.to_raw_data(), face.get_width(), face.get_height()))
end
# Display
var is_closed=false
while !app.is_closed()
    app.prepare()
    imgui.begin_window("Frontal face detect", is_closed, {})
    for i=0, i<img_list.size(), ++i
        imgui.image(img_list.at(i), imgui.vec2(200, 200))
        if i%5!=4
            imgui.same_line()
        end
    end
    imgui.set_window_size(imgui.vec2(app.get_window_width(), app.get_window_height()))
    imgui.set_window_pos(imgui.vec2(0,0))
    imgui.end_window()
    app.render()
end