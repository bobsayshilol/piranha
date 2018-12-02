#include <demos.h>

using namespace manta;

void manta_demo::teapotDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
	Scene scene;

	// Load all object files
	ObjFileLoader smallHouseObj;
	bool result = smallHouseObj.readObjFile(MODEL_PATH "small_house.obj");

	ObjFileLoader teapotObj;
	result = teapotObj.readObjFile(MODEL_PATH "teapot.obj");

	// Create all materials
	SimpleSpecularDiffuseMaterial wallMaterial;
	wallMaterial.setEmission(math::constants::Zero);
	wallMaterial.setDiffuseColor(getColor(200, 200, 200));
	wallMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorLight;
	outdoorLight.setEmission(math::loadVector(9, 8, 8));
	outdoorLight.setDiffuseColor(math::constants::Zero);
	outdoorLight.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial outdoorTopLightMaterial;
	outdoorTopLightMaterial.setEmission(math::loadVector(10, 10, 11));
	outdoorTopLightMaterial.setDiffuseColor(math::constants::Zero);
	outdoorTopLightMaterial.setSpecularColor(math::constants::Zero);

	SimpleSpecularDiffuseMaterial teapotMaterial;
	teapotMaterial.setEmission(math::constants::Zero);
	teapotMaterial.setDiffuseColor(getColor(1, 1, 1));
	teapotMaterial.setSpecularColor(getColor(100, 100, 100));

	SimpleSpecularDiffuseMaterial groundMaterial;
	groundMaterial.setEmission(math::constants::Zero);
	groundMaterial.setDiffuseColor(math::mul(math::loadVector(78, 46, 40), math::loadScalar(0.001)));
	groundMaterial.setSpecularColor(math::constants::Zero);

	// Create all scene geometry
	Mesh smallHouse;
	smallHouse.loadObjFileData(&smallHouseObj);
	smallHouse.setFastIntersectEnabled(false);
	smallHouse.setFastIntersectRadius((math::real)4.0);

	Mesh teapot;
	teapot.loadObjFileData(&teapotObj);
	teapot.setFastIntersectEnabled(false);
	teapot.setFastIntersectRadius((math::real)4.0);

	SpherePrimitive outdoorLightGeometry;
	outdoorLightGeometry.setRadius((math::real)10.0);
	outdoorLightGeometry.setPosition(math::loadVector(10.5, 0.0, -20.5));

	SpherePrimitive outdoorTopLightGeometry;
	outdoorTopLightGeometry.setRadius((math::real)10.0);
	outdoorTopLightGeometry.setPosition(math::loadVector(0.0, 25.0, 2));

	SpherePrimitive groundGeometry;
	groundGeometry.setRadius((math::real)50000.0);
	groundGeometry.setPosition(math::loadVector(0.0, -50000.1, 0));

	// Create scene objects
	//SceneObject *smallHouseObject = scene.createSceneObject();
	//smallHouseObject->setGeometry(&smallHouse);
	//smallHouseObject->setMaterial(&wallMaterial);

	SceneObject *teapotObject = scene.createSceneObject();
	teapotObject->setGeometry(&teapot);
	teapotObject->setMaterial(&teapotMaterial);

	//SceneObject *ground = scene.createSceneObject();
	//ground->setGeometry(&groundGeometry);
	//ground->setMaterial(&groundMaterial);

	//SceneObject *outdoorTopLightObject = scene.createSceneObject();
	//outdoorTopLightObject->setGeometry(&outdoorTopLightGeometry);
	//outdoorTopLightObject->setMaterial(&outdoorTopLightMaterial);

	//SceneObject *lightSource = scene.createSceneObject();
	//lightSource->setGeometry(&outdoorLightGeometry);
	//lightSource->setMaterial(&outdoorLight);

	// Create the camera
	CameraRayEmitterGroup camera;
	camera.setSamplingWidth(1);
	camera.setDirection(math::loadVector(-1.0, 0.0, 0.0));
	camera.setPosition(math::loadVector(5.0, 2.0, 0.0));
	camera.setUp(math::loadVector(0.0f, 1.0, 0.0));
	camera.setPlaneDistance(1.0f);
	camera.setPlaneHeight(1.0f);
	camera.setResolutionX(resolutionX);
	camera.setResolutionY(resolutionY);
	camera.setSamplesPerPixel(samplesPerPixel);

	// Create the raytracer
	RayTracer rayTracer;
	rayTracer.initialize(500 * MB, 50 * MB, 12, 10000, true);
	rayTracer.setBackgroundColor(getColor(135, 206, 235));
	rayTracer.setDeterministicSeedMode(true);
	//rayTracer.traceAll(&scene, &camera);
	rayTracer.tracePixel(817, 689, &scene, &camera);

	// Output the results to file
	math::Vector *pixels = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * resolutionX * resolutionY, 16);

	for (int i = 0; i < resolutionY; i++) {
		for (int j = 0; j < resolutionX; j++) {
			math::Vector v = ((CameraRayEmitter *)(camera.getEmitters()[i * resolutionX + j]))->getIntensity();
			math::real r = math::getX(v);
			math::real g = math::getY(v);
			math::real b = math::getZ(v);

			pixels[i * resolutionX + j] = v;
		}
	}

	// Clean everything up
	for (int i = camera.getEmitterCount() - 1; i >= 0; i--) {
		((CameraRayEmitter *)(camera.getEmitters()[i]))->destroyRays();
	}

	SaveImageData(pixels, resolutionX, resolutionY, createUniqueRenderFilename(RENDER_OUTPUT, "teapot_demo", samplesPerPixel).c_str());
	camera.destroyEmitters();

	rayTracer.destroy();
}