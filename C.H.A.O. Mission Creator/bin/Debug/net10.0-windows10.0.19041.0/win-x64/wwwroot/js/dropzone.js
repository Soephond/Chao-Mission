window.dropzoneInterop = {
    _handlers: {},

    init(elementId, dotnetRef) {
        const el = document.getElementById(elementId);
        if (!el) return;

        const onDragOver = e => { e.preventDefault(); el.classList.add('drag-over'); };
        const onDragLeave = () => el.classList.remove('drag-over');
        const onDrop = async e => {
            e.preventDefault();
            el.classList.remove('drag-over');
            const file = e.dataTransfer?.files?.[0];
            if (!file || !file.name.endsWith('.json')) return;
            const text = await file.text();
            dotnetRef.invokeMethodAsync('OnFileDropped', file.name, text);
        };

        el.addEventListener('dragover', onDragOver);
        el.addEventListener('dragleave', onDragLeave);
        el.addEventListener('drop', onDrop);
        this._handlers[elementId] = { onDragOver, onDragLeave, onDrop };
    },

    dispose(elementId) {
        const el = document.getElementById(elementId);
        const h = this._handlers[elementId];
        if (el && h) {
            el.removeEventListener('dragover', h.onDragOver);
            el.removeEventListener('dragleave', h.onDragLeave);
            el.removeEventListener('drop', h.onDrop);
        }
        delete this._handlers[elementId];
    }
};