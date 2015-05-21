#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume16Reader.h>
#include <vtkVolume.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkCamera.h>

#include <vtkTIFFReader.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageIslandRemoval2D.h>
#include <vtkImageThreshold.h>
#include <vtkImageShrink3D.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkPolyDataMapper.h>
#include <vtkExtractVOI.h>
#include <vtkLODActor.h>
#include <vtkContourFilter.h>
#include <vtkSliceCubes.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0]
			<< " Filename(.tif)" << std::endl;
		return EXIT_FAILURE;
	}

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkTIFFReader> imageReader = vtkSmartPointer<vtkTIFFReader>::New();
	imageReader->SetFileName(argv[1]);
	imageReader->SetFileDimensionality(3);
	imageReader->SetDataExtent(0, 499, 0, 469, 0, 135);
	imageReader->SetDataSpacing(1, 1, 1);
	imageReader->SetNumberOfScalarComponents(1);
	imageReader->SetDataByteOrderToLittleEndian();
	imageReader->ReleaseDataFlagOn();
	imageReader->Update();

	/*vtkSmartPointer<vtkImageIslandRemoval2D> islandRemovalFilter =
		vtkSmartPointer<vtkImageIslandRemoval2D>::New();
	islandRemovalFilter->SetAreaThreshold(5);
	islandRemovalFilter->SetIslandValue(255.0);
	islandRemovalFilter->SetReplaceValue(0);
	islandRemovalFilter->SetInputConnection(imageReader->GetOutputPort());
	islandRemovalFilter->ReleaseDataFlagOn();
	islandRemovalFilter->Update();


	vtkSmartPointer<vtkImageThreshold> imageThreshold = vtkSmartPointer<vtkImageThreshold>::New();
	imageThreshold->SetInputConnection(islandRemovalFilter->GetOutputPort());
	imageThreshold->ThresholdByLower(0.1);
	imageThreshold->ReplaceInOn();
	imageThreshold->SetInValue(0);
	imageThreshold->ReleaseDataFlagOn();
	imageThreshold->Update();

	vtkSmartPointer<vtkImageShrink3D> shrink = vtkSmartPointer<vtkImageShrink3D>::New();
	shrink->SetInputConnection(imageThreshold->GetOutputPort());
	shrink->SetShrinkFactors(0.1, 0.1, 0.1);
	shrink->ReleaseDataFlagOn();
	shrink->Update();*/

	vtkSmartPointer<vtkImageGaussianSmooth> gaussianSmooth = vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gaussianSmooth->SetInputConnection(imageReader->GetOutputPort());
	gaussianSmooth->SetDimensionality(3);
	gaussianSmooth->Update();

	//vtkSmartPointer<vtkMarchingCubes> contour = vtkSmartPointer<vtkMarchingCubes>::New();
	vtkContourFilter *contour = vtkContourFilter::New();
	contour->SetInputConnection(gaussianSmooth->GetOutputPort());
	contour->SetValue(0, 50);
	contour->ComputeGradientsOn();
	contour->ComputeScalarsOff();
	contour->Update();

	vtkSmartPointer<vtkPolyDataMapper> polyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	polyDataMapper->SetInputConnection(contour->GetOutputPort());
	polyDataMapper->ScalarVisibilityOff();
	polyDataMapper->ReleaseDataFlagOn();
	polyDataMapper->Update();

	vtkSmartPointer<vtkLODActor> actor = vtkSmartPointer<vtkLODActor>::New();
	actor->SetNumberOfCloudPoints(1000000);
	actor->SetMapper(polyDataMapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.329412, 0.34902, 0.427451);
	renderer->AddActor(actor);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(250, 250);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	interactor->Initialize();
	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;

}