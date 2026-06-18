import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import { viteSingleFile } from "vite-plugin-singlefile";
import tailwindcss from "@tailwindcss/vite";

// https://vite.dev/config/
export default defineConfig({
  base: "./",
  plugins: [react(), viteSingleFile(), tailwindcss()],
  build: {
    cssCodeSplit: false,
    assetsInlineLimit: 100000000,
    rolldownOptions: {
      output: {
        manualChunks: undefined,
      },
    },
  },
});
