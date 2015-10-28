#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#define PI 3.14159265

class Camera {
private:
    float lst;
    float cur;

    float cpx;
    float cpy;

    float wdt;
    float hgt;

    float fov;
    float asp;
    float znr;
    float zfr;

    glm::vec2 rot;
    float rfa;
    float tfa;
    glm::vec3 eye;
    glm::vec3 dir;

public:
    Camera(float, float, float);

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();

    void setFovy(float);
    void setNearZ(float);
    void setFarZ(float);
    void setPosition(glm::vec3);
    void setDirection(glm::vec3);
    void setUpAngle(glm::vec3);

    void update(float, float, float, bool*);
};

Camera::Camera(float start, float width, float height) {
    wdt = width;
    hgt = height;
    cpx = 0;
    cpy = 0;
    rot = glm::vec2(0., -PI);
    fov = 75.0;
    asp = width/height;
    znr = 0.1;
    zfr = 100.0;
    rfa = 0.1;
    tfa = 3.0;
    eye = glm::vec3(0.f, 0.4f, 0.f);
    lst = start;
    dir = glm::vec3(0.f, 0.f, -1.f);
}

void Camera::update(float time, float cursorX, float cursorY, bool* keys) {
    cur = time;
    float tDif = cur-lst;

    glm::vec2 dv = glm::vec2(cursorX-cpx, cursorY-cpy);
    rot -= rfa*dv;
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(0.0f, 1.0f, 0.0f))
     * glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(1.0, 0.0f, 0.0f));
    glm::vec3 RZ = glm::vec3(R[2][0], R[2][1], R[2][2]);
    glm::vec3 RX = glm::vec3(R[0][0], R[0][1], R[0][2]);

    if (keys['W']) eye -= RZ*tDif*tfa;
    if (keys['S']) eye += RZ*tDif*tfa;
    if (keys['D']) eye -= RX*tDif*tfa;
    if (keys['A']) eye += RX*tDif*tfa;

    lst = cur;

    cpx = cursorX;
    cpy = cursorY;
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(fov, asp, znr, zfr);
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), eye);
    glm::mat4 R = glm::rotate(glm::mat4(1.0f),rot.x, glm::vec3(0.0f, 1.0f, 0.0f))*
        glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(1.0f,0.0f,0.0f));

    glm::mat4 C = T*R;
    return glm::inverse(C);
}

glm::vec3 Camera::getPosition() {
	return glm::vec3(eye);
}

void Camera::setFovy(float fovy) {
	fov = fovy;
}

void Camera::setNearZ(float zNear) {
	znr = zNear;
}

void Camera::setFarZ(float zFar) {
	zfr = zFar;
}

void Camera::setPosition(glm::vec3 position) {
	eye = glm::vec3(position);
}

void Camera::setDirection(glm::vec3 direction) {
	dir = glm::vec3(direction);
}

#endif
