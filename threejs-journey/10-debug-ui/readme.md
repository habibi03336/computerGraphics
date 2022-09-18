# Three.js Journey

원하는 장면을 만들기 위해서는 다양한 설정값을 시도해보는 작업이 필요하다.
이를 UI Debugging이라고 한다. 

이 작업을 위해서 다양한 라이브러리를 활용할 수 있는데 이번 수업에서는 lil-gui를 활용했다.

## lil-gui 사용 예시
```javascript
import GUI from 'lil-gui';

// control panel이 화면에 표시된다.
const gui = new GUI();

const geometry = new THREE.BoxGeometry(1, 1, 1)
const material = new THREE.MeshBasicMaterial({ color: 0xff0000 })
const mesh = new THREE.Mesh(geometry, material)


//control할 속성들을 정의한다.

// mesh.position의 'y' 속성(number)을 컨트롤
gui
    .add(mesh.position, 'y')
    .min(-3)
    .max(3)
    .step(0.01)
    .name('elevation')

// mesh의 'visible' 속성(boolean)을 컨트롤
gui
    .add(mesh, 'visible')

// mesh.material의 'wireframe' 속성(boolean)을 컨트롤
gui
    .add(mesh.material, 'wireframe')

// mesh.material의 'color' 속성을 컨트롤 (color의 경우 값의 도메인이 특징적이므로 이를 위한 '.addColor' 메소드를 활용한다.)
gui
    .addColor(mesh.material, 'color')


// 다음과 같이 클릭 이벹느에 대한 handler를 등록할 수도 있다. 
const parameters = {
    handler : () => { console.log("hello world") }
}
gui
    .add(parameters, "handler")
```

## Tip
프로젝트를 진행하면서 지속적으로 debugging 설정을 해주자. 한 번에 하려고 하면 매우 힘들고, 중간 중간 자유롭게 확인하면서 작업하면 더 좋은 결과물이 나올 것이다.

