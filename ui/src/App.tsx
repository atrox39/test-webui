function App() {

  const native = async () => {
    try {
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      const res = await (window as any).Nativo_Alerta();
      console.log(res);
    } catch (error) {
      console.error(error);
    }
  };

  return (
    <>
      <button type="button" onClick={native}>
        Press Me
      </button>
    </>
  )
}

export default App
