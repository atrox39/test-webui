function App() {
  return (
    <div className="flex min-h-[calc(100vh-4rem)] items-center justify-center px-6 py-10">
      <div className="w-full max-w-md rounded-2xl border border-white/10 bg-white/[0.04] p-7 shadow-[0_20px_80px_rgba(0,0,0,0.55)] backdrop-blur">
        <div className="flex items-center justify-between gap-4">
          <h1 className="text-balance text-2xl font-semibold tracking-tight">
            Test WebUI
          </h1>
          <span className="inline-flex items-center rounded-full border border-white/10 bg-white/5 px-2.5 py-1 text-xs text-slate-200">
            WebView2
          </span>
        </div>

        <p className="mt-3 text-pretty text-sm leading-relaxed text-slate-300">
          Aplicación de escritorio Windows que integra React/Vite en WebView2
          para probar comunicación entre JavaScript y C++.
        </p>

        <div className="mt-6 h-px w-full bg-gradient-to-r from-transparent via-white/10 to-transparent" />

        <div className="mt-5 flex items-center gap-2 text-xs text-slate-400">
          <span className="h-1.5 w-1.5 rounded-full bg-emerald-400/80" />
          Listo para ejecutar en modo Debug o Release
        </div>
      </div>
    </div>
  );
}

export default App;
