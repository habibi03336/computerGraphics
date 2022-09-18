# Three.js Journey

## Geometry
Geometry는 vertices의 모음을 통해 어떤 모양을 형성한 것을 의미한다.

## [Buffer Geometry](https://threejs.org/docs/#api/en/core/BufferGeometry)
모든 built in Three.js geometry는 이 클래스를 상속받는다.

## 대표적인 Built-in Geometry
1. [Box](https://threejs.org/docs/#api/en/geometries/BoxGeometry)
2. [Plane](https://threejs.org/docs/#api/en/geometries/PlaneGeometry)
3. [Circle](https://threejs.org/docs/#api/en/geometries/CircleGeometry)
4. [Cone](https://threejs.org/docs/#api/en/geometries/ConeGeometry)
5. [Cylinder](https://threejs.org/docs/#api/en/geometries/CylinderGeometry)
6. [Ring](https://threejs.org/docs/#api/en/geometries/RingGeometry)
7. [Torus](https://threejs.org/docs/#api/en/geometries/TorusGeometry)
8. [TorusKnot](https://threejs.org/docs/#api/en/geometries/TorusKnotGeometry)
9. [Dodecahedron](https://threejs.org/docs/#api/en/geometries/DodecahedronGeometry)
10. [Octahedron](https://threejs.org/docs/#api/en/geometries/OctahedronGeometry)
11. [TetrahedronGeometry](https://threejs.org/docs/#api/en/geometries/TetrahedronGeometry)
12. [Icosahedron](https://threejs.org/docs/#api/en/geometries/IcosahedronGeometry)
13. [Sphere](https://threejs.org/docs/#api/en/geometries/SphereGeometry)
14. [Shape](https://threejs.org/docs/#api/en/geometries/ShapeGeometry)
15. [Tube](https://threejs.org/docs/#api/en/geometries/TubeGeometry)
16. [Extrude](https://threejs.org/docs/#api/en/geometries/ExtrudeGeometry)
17. [Lathe](https://threejs.org/docs/#api/en/geometries/LatheGeometry)
18. [Text](https://threejs.org/docs/?q=textge#examples/en/geometries/TextGeometry)

## Three.js에서 vertices 정보를 만들고 전달하기.

```javascript
// vertices 정보를 담은 array를 생성한다. Float32Array를 활용한다.
const positionsArray = new Float32Array([
    0, 0, 0,
    0, 1, 0,
    1, 0, 0
])

//하나의 vertice 정보에 대한 속성 값을 설정한다. (요소 3개가 하나의 vertice에 대한 정보이다.)
const positionsAttribute = new THREE.BufferAttribute(positionsArray, 3)

//shader의 position 변수에 position attribute를 전달한다. 
const geometry = new THREE.BufferGeometry()
geometry.setAttribute('position', positionsAttribute)
```