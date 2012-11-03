#ifndef __NOUVEAU_BSP_H__
#define __NOUVEAU_BSP_H__

#include <core/engine.h>
#include <core/engctx.h>

struct nouveau_bsp_chan {
	struct nouveau_engctx base;
};
INHERITS_NV_ENGCTX(nv_bspch, struct nouveau_bsp_chan);

#define INHERITS_NV_BSP_CHAN(pfx, type) \
	NOUVEAU_UPCAST(pfx, bspch, type, struct nouveau_bsp_chan) \
	INHERITS_NV_ENGCTX(pfx, type)

#define nouveau_bsp_context_create(p,e,c,g,s,a,f,d)                            \
	nouveau_engctx_create((p), (e), (c), (g), (s), (a), (f), (d))
#define nouveau_bsp_context_destroy(d)                                         \
	nouveau_engctx_destroy(&(d)->base)
#define nouveau_bsp_context_init(d)                                            \
	nouveau_engctx_init(&(d)->base)
#define nouveau_bsp_context_fini(d,s)                                          \
	nouveau_engctx_fini(&(d)->base, (s))

#define _nouveau_bsp_context_dtor _nouveau_engctx_dtor
#define _nouveau_bsp_context_init _nouveau_engctx_init
#define _nouveau_bsp_context_fini _nouveau_engctx_fini
#define _nouveau_bsp_context_rd32 _nouveau_engctx_rd32
#define _nouveau_bsp_context_wr32 _nouveau_engctx_wr32

struct nouveau_bsp {
	struct nouveau_engine base;
};
INHERITS_NV_ENGINE(nv_bsp, struct nouveau_bsp);

#define INHERITS_NV_BSP(pfx, type) \
	NOUVEAU_UPCAST(pfx, bsp, type, struct nouveau_bsp); \
	INHERITS_NV_ENGINE(pfx, type)

#define nouveau_bsp_create(p,e,c,d)                                            \
	nouveau_engine_create((p), (e), (c), true, "PBSP", "bsp", (d))
#define nouveau_bsp_destroy(d)                                                 \
	nouveau_engine_destroy(&(d)->base)
#define nouveau_bsp_init(d)                                                    \
	nouveau_engine_init(&(d)->base)
#define nouveau_bsp_fini(d,s)                                                  \
	nouveau_engine_fini(&(d)->base, (s))

#define _nouveau_bsp_dtor _nouveau_engine_dtor
#define _nouveau_bsp_init _nouveau_engine_init
#define _nouveau_bsp_fini _nouveau_engine_fini

extern struct nouveau_oclass nv84_bsp_oclass;

#endif
