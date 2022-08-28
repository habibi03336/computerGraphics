import './style.css'
import * as THREE from 'three'

// Canvas
const canvas = document.querySelector('canvas.webgl')

// Scene
const scene = new THREE.Scene()

/**
 * Objects
 */
const geometry = new THREE.BoxGeometry(1, 1, 1)
const material = new THREE.MeshBasicMaterial({ color: 0xff0000 })
const mesh = new THREE.Mesh(geometry, material)

//Position ( Vector 3 )
mesh.position.x = 0.7
mesh.position.y = -0.6
mesh.position.z = -3


//Scale ( Vector 3 )
mesh.scale.set(1,2,3)

//Rotation using rotation property
mesh.rotation.reorder('YXZ')
mesh.rotation.set(0, 0 , 0)

scene.add(mesh)

const group = new THREE.Group()

scene.add(group)

const cube1 = new THREE.Mesh( 
    new THREE.BoxGeometry(1,1,1), 
    new THREE.MeshBasicMaterial({ color: 0xeeeeee })
)

group.add(cube1)

const cube2 = new THREE.Mesh(
    new THREE.BoxGeometry(1,1,1),
    new THREE.MeshBasicMaterial({ color: 0x0000ff })
)

cube2.position.x = -2
group.add(cube2)

const cube3 = new THREE.Mesh(
    new THREE.BoxGeometry(1,1,1),
    new THREE.MeshBasicMaterial({ color: 0x00ff00 })
)

cube3.position.x = 2
group.add(cube3)

group.position.y = 1
group.rotation.y = 1

const axesHelper = new THREE.AxesHelper(1)

scene.add(axesHelper)

/**
 * Sizes
 */
const sizes = {
    width: 800,
    height: 600
}


/**
 * Camera
 */
const camera = new THREE.PerspectiveCamera(75, sizes.width / sizes.height)
camera.position.z = 3
camera.position.y = 0
camera.position.x = 0

scene.add(camera)

/**
 * Renderer
 */
const renderer = new THREE.WebGLRenderer({
    canvas: canvas
})

renderer.setSize(sizes.width, sizes.height)
renderer.render(scene, camera)


