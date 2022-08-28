import './style.css'
import * as THREE from "three"

// Scene
// Scene(World Space)을 생성한다. 
const scene = new THREE.Scene()

// Red Cube
// width == height == depth == 1인 Box Geometry를 생성한다.
const geometry = new THREE.BoxGeometry(1, 1, 1)
// Material을 생성한다.
const material = new THREE.MeshBasicMaterial({ color: 0xff0000 })
// mesh(geometry와 material의 조합)을 생성한다.
const mesh = new THREE.Mesh(geometry, material)

// Scene에 mesh를 추가한다.
scene.add(mesh)

// Sizes
const sizes = {
    width: 800,
    height: 600
}

// Camera(Point of view)를 생성한다.
const camera = new THREE.PerspectiveCamera(75, sizes.width / sizes.height) // fov(of vertical vision), aspect ratio
// move camera 3 on z-axis (right-hand coordinates)
camera.position.z = 3
camera.position.x = 1
camera.position.y = 1


scene.add(camera)

// Renderer
const renderer = new THREE.WebGLRenderer({
    canvas: document.querySelector('canvas.webgl')
})

// Renderer의 사이즈를 바꿔준다.
renderer.setSize(sizes.width, sizes.height)

// renderer에 render의 대상이 되는 scene과 카메라를 넘겨준다.
renderer.render(scene, camera)