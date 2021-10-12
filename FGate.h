the gate node 
is an struct -> empty class
FGate.h

USTRUCT(BlueprintType)
struct FGate
{
	GENERATED_BODY()
	
	public:
	FORCEINLINE FGate();
	// ver explicit --> 
	explicit FORCEINLINE FGate(bool bStartClosed);
	
	FORCEINLINE void Open() {bGateOpen =true;}
	FORCEINLINE void Close() {bGateOpen =false;}
	
	FORCEINLINE void Toggle() {bGateOpen != bGateOpen;}
	
	FORCEINLINE bool IsOpen() const {return bGateOpen;}
	
	private:
	UPROPERTY(VisibleAnywhere)
		bool bGateOpen;
};
//constructors	
	FORCEINLINE FGate::FGate():bGateOpen(false){}
	FORCEINLINE FGate::FGate(const bool bStartClosed):bGateOpen(!bStartClosed){}	



para implementarse ---

// al estar declarado como Explicit - necesito llamar al constructor. 
// si no estuviera el explicit , podria decir Gate =false ;  pero eso se torna muy poco saludable a la lectura.
FGate Gate = FGate(false);

y luego llamamos 
Gate.Open();
Gate.Close();

if(Gate.IsOpen()){funciona el stream}