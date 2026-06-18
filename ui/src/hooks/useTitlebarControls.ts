import { useRef } from "react";
import type { PointerEvent } from "react";

type DragState = {
  active: boolean;
  pointerId: number;
  x: number;
  y: number;
};

export function useTitlebarControls() {
  const dragRef = useRef<DragState>({
    active: false,
    pointerId: -1,
    x: 0,
    y: 0,
  });

  const onTitlebarPointerDown = (e: PointerEvent<HTMLDivElement>) => {
    if (e.button !== 0) return;
    dragRef.current.active = true;
    dragRef.current.pointerId = e.pointerId;
    dragRef.current.x = e.clientX;
    dragRef.current.y = e.clientY;
  };

  const onTitlebarPointerMove = (e: PointerEvent<HTMLDivElement>) => {
    if (!dragRef.current.active) return;
    if (dragRef.current.pointerId !== e.pointerId) return;

    const dx = Math.abs(e.clientX - dragRef.current.x);
    const dy = Math.abs(e.clientY - dragRef.current.y);
    if (dx < 3 && dy < 3) return;

    dragRef.current.active = false;
    window.Native_BeginDrag?.();
  };

  const onTitlebarPointerUpOrCancel = () => {
    dragRef.current.active = false;
  };

  const onTitlebarDoubleClick = () => {
    dragRef.current.active = false;
    window.Native_ToggleMaximize?.();
  };

  const minimize = (e: PointerEvent<HTMLButtonElement>) => {
    e.stopPropagation();
    window.Native_Minimize?.();
  };

  const maximize = (e: PointerEvent<HTMLButtonElement>) => {
    e.stopPropagation();
    window.Native_ToggleMaximize?.();
  };

  const close = (e: PointerEvent<HTMLButtonElement>) => {
    e.stopPropagation();
    window.Native_Close?.();
  };

  return {
    titlebar: {
      onPointerDown: onTitlebarPointerDown,
      onPointerMove: onTitlebarPointerMove,
      onPointerUp: onTitlebarPointerUpOrCancel,
      onPointerCancel: onTitlebarPointerUpOrCancel,
      onDoubleClick: onTitlebarDoubleClick,
    },
    buttons: {
      minimize,
      maximize,
      close,
    },
  };
}

declare global {
  interface Window {
    Native_BeginDrag?: () => void;
    Native_ToggleMaximize?: () => void;
    Native_Minimize?: () => void;
    Native_Close?: () => void;
  }
}
