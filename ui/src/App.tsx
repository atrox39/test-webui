import type { CSSProperties } from "react";
import { useTitlebarControls } from "./hooks/useTitlebarControls";

type AppRegionStyle = CSSProperties & { WebkitAppRegion?: "drag" | "no-drag" };

const dragRegionStyle: AppRegionStyle = {
  WebkitAppRegion: "drag",
  WebkitUserSelect: "none",
  userSelect: "none",
};

const noDragRegionStyle: AppRegionStyle = {
  WebkitAppRegion: "no-drag",
};

function App() {
  const { titlebar, buttons } = useTitlebarControls();

  return (
    <div className="min-h-screen bg-slate-950 text-slate-100">
      <div className="pointer-events-none absolute inset-0 overflow-hidden">
        <div className="absolute left-1/2 top-[-12rem] h-[28rem] w-[28rem] -translate-x-1/2 rounded-full bg-cyan-500/20 blur-3xl" />
        <div className="absolute bottom-[-14rem] right-[-10rem] h-[32rem] w-[32rem] rounded-full bg-fuchsia-500/15 blur-3xl" />
        <div className="absolute inset-0 bg-[radial-gradient(1200px_600px_at_50%_-200px,rgba(255,255,255,0.08),transparent_60%)]" />
      </div>

      <div className="fixed inset-x-0 top-0 z-10 h-12 border-b border-white/10 bg-slate-950/40 backdrop-blur">
        <div
          className="flex h-full items-center justify-between px-3"
          style={dragRegionStyle}
          onPointerDown={titlebar.onPointerDown}
          onPointerMove={titlebar.onPointerMove}
          onPointerUp={titlebar.onPointerUp}
          onPointerCancel={titlebar.onPointerCancel}
          onDoubleClick={titlebar.onDoubleClick}
        >
          <div className="flex items-center gap-2">
            <div className="h-2.5 w-2.5 rounded-full bg-gradient-to-br from-cyan-300 to-fuchsia-300" />
            <div className="text-xs font-medium tracking-wide text-slate-200">
              Test WebUI
            </div>
          </div>

          <div className="flex items-center" style={noDragRegionStyle}>
            <button
              type="button"
              onPointerDown={buttons.minimize}
              className="group inline-flex h-9 w-10 items-center justify-center rounded-md text-slate-200/80 transition hover:bg-white/5 hover:text-slate-100 active:bg-white/10"
              aria-label="Minimize"
            >
              <div className="h-px w-3 bg-current opacity-80" />
            </button>

            <button
              type="button"
              onPointerDown={buttons.maximize}
              className="group inline-flex h-9 w-10 items-center justify-center rounded-md text-slate-200/80 transition hover:bg-white/5 hover:text-slate-100 active:bg-white/10"
              aria-label="Maximize"
            >
              <div className="h-3 w-3 rounded-[3px] border border-current opacity-80" />
            </button>

            <button
              type="button"
              onPointerDown={buttons.close}
              className="group inline-flex h-9 w-10 items-center justify-center rounded-md text-slate-200/80 transition hover:bg-red-500/15 hover:text-red-200 active:bg-red-500/25"
              aria-label="Close"
            >
              <div className="relative h-3 w-3 opacity-80">
                <div className="absolute left-1/2 top-1/2 h-px w-3 -translate-x-1/2 -translate-y-1/2 rotate-45 bg-current" />
                <div className="absolute left-1/2 top-1/2 h-px w-3 -translate-x-1/2 -translate-y-1/2 -rotate-45 bg-current" />
              </div>
            </button>
          </div>
        </div>
      </div>

      <div className="relative flex min-h-screen items-center justify-center px-6 py-10 pt-16">
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
    </div>
  );
}

export default App;
