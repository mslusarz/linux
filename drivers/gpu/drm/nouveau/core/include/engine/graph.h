#ifndef __NOUVEAU_GRAPH_H__
#define __NOUVEAU_GRAPH_H__

#include <core/engine.h>
#include <core/engctx.h>
#include <core/enum.h>

struct nouveau_graph_chan {
	struct nouveau_engctx base;
};
INHERITS_NV_ENGCTX(nv_grchan, struct nouveau_graph_chan);

#define INHERITS_NV_GRCHAN(pfx, type) \
	NOUVEAU_UPCAST(pfx, grchan, type, struct nouveau_graph_chan); \
	INHERITS_NV_ENGCTX(pfx, type) \
	NOUVEAU_OBJECT_HELPERS(pfx, type)

#define nouveau_graph_context_create(p,e,c,g,s,a,f,d)                          \
	nouveau_engctx_create((p), (e), (c), (g), (s), (a), (f), (d))
#define nouveau_graph_context_destroy(d)                                       \
	nouveau_engctx_destroy(&(d)->base)
#define nouveau_graph_context_init(d)                                          \
	nouveau_engctx_init(&(d)->base)
#define nouveau_graph_context_fini(d,s)                                        \
	nouveau_engctx_fini(&(d)->base, (s))

#define _nouveau_graph_context_dtor _nouveau_engctx_dtor
#define _nouveau_graph_context_init _nouveau_engctx_init
#define _nouveau_graph_context_fini _nouveau_engctx_fini
#define _nouveau_graph_context_rd32 _nouveau_engctx_rd32
#define _nouveau_graph_context_wr32 _nouveau_engctx_wr32

struct nouveau_graph {
	struct nouveau_engine base;
};
INHERITS_NV_ENGINE(nv_graph, struct nouveau_graph);

#define INHERITS_NV_GRAPH(pfx, type) \
	NOUVEAU_UPCAST(pfx, graph, type, struct nouveau_graph) \
	INHERITS_NV_ENGINE(pfx, type)

static inline struct nouveau_graph *
nouveau_graph(void *obj)
{
	return (void *)nv_device(obj)->subdev[NVDEV_ENGINE_GR];
}

#define nouveau_graph_create(p,e,c,y,d)                                        \
	nouveau_engine_create((p), (e), (c), (y), "PGRAPH", "graphics", (d))
#define nouveau_graph_destroy(d)                                               \
	nouveau_engine_destroy(&(d)->base)
#define nouveau_graph_init(d)                                                  \
	nouveau_engine_init(&(d)->base)
#define nouveau_graph_fini(d,s)                                                \
	nouveau_engine_fini(&(d)->base, (s))

#define _nouveau_graph_dtor _nouveau_engine_dtor
#define _nouveau_graph_init _nouveau_engine_init
#define _nouveau_graph_fini _nouveau_engine_fini

extern struct nouveau_oclass nv04_graph_oclass;
extern struct nouveau_oclass nv10_graph_oclass;
extern struct nouveau_oclass nv20_graph_oclass;
extern struct nouveau_oclass nv25_graph_oclass;
extern struct nouveau_oclass nv2a_graph_oclass;
extern struct nouveau_oclass nv30_graph_oclass;
extern struct nouveau_oclass nv34_graph_oclass;
extern struct nouveau_oclass nv35_graph_oclass;
extern struct nouveau_oclass nv40_graph_oclass;
extern struct nouveau_oclass nv50_graph_oclass;
extern struct nouveau_oclass nvc0_graph_oclass;
extern struct nouveau_oclass nve0_graph_oclass;

extern const struct nouveau_bitfield nv04_graph_nsource[];
extern struct nouveau_ofuncs nv04_graph_ofuncs;
bool nv04_graph_idle(void *obj);

extern const struct nouveau_bitfield nv10_graph_intr_name[];
extern const struct nouveau_bitfield nv10_graph_nstatus[];

extern const struct nouveau_enum nv50_data_error_names[];

#endif
