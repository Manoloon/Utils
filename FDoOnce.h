the Do Once node 
is an struct -> empty class
FDoOnce.h

USTRUCT(BlueprintType)
struct FDoOnce
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
		bool bDoOnce;
		
	FORCEINLINE FDoOnce();
	// ver explicit --> 
	explicit FORCEINLINE FDoOnce(bool bStartClosed);
	
	FORCEINLINE void Reset() {bDoOnce =true;}		
	FORCEINLINE bool Execute()
	{
		if(bDoOnce)
		{
			bDoOnce = false;
			return true;
		}
		else
		{
			return false;
		}
	}
};
//constructors	
	FORCEINLINE FDoOnce::FDoOnce():bDoOnce(false){}
	FORCEINLINE FDoOnce::FDoOnce(const bool bStartClosed):bDoOnce(!bStartClosed){}	



para implementarse ---

// al estar declarado como Explicit - necesito llamar al constructor. 
// si no estuviera el explicit , podria decir Gate =false ;  pero eso se torna muy poco saludable a la lectura.
FDoOnce DoOnce = FDoOnce(false);

y luego llamamos 
DoOnce.Reset()
if(DoOnce.Execute()){ do whatever};

if(Gate.IsOpen()){funciona el stream}