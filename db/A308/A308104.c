/*
 * A308104  Distinct values taken by the DENEAT operator (A073053) in
 *          increasing order.
 * https://oeis.org/A308104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308104: distinct values of the DENEAT operator e.o(e+o),
   sorted increasingly.  a(n) = the n-th distinct value of
   concat(e, o, e+o) for e, o ≥ 0, e+o > 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- helpers for decimal length and powers of 10 ---------- */

static idx_t digits_idx(seq_t x)
{
    if (x == 0) return 1;
    idx_t d = 0;
    while (x > 0) { x /= 10; d++; }
    return d;
}

static __uint128_t pow10_128(idx_t exp)
{
    __uint128_t r = 1;
    while (exp--) r *= 10;
    return r;
}

/* Return true if the concatenation fits into seq_t;  otherwise *out
   is left unchanged.  The value is stored in *out when true. */
static bool concat_fits(seq_t *out, idx_t e, idx_t o)
{
    idx_t s      = e + o;
    idx_t o_len  = digits_idx((seq_t)o);
    idx_t s_len  = digits_idx((seq_t)s);

    __uint128_t val = (__uint128_t)e;
    val *= pow10_128(o_len + s_len);
    val += (__uint128_t)o * pow10_128(s_len);
    val += (__uint128_t)s;

    const __uint128_t max_seq = (unsigned __int128)(seq_t)-1u;
    if (val > max_seq) return false;

    *out = (seq_t)val;
    return true;
}

static int cmp_seq(const seq_t *x, const seq_t *y)
{
    return (*x < *y) ? -1 : (*x > *y);
}

/* ---------- heap structure for generating values in sorted order ---------- */

typedef struct {
    seq_t val;   /* concatenated integer */
    idx_t e;     /* fixed e for this stream */
    idx_t o;     /* current o for this stream */
} heap_node_t;

static void heap_push(heap_node_t *h, size_t *sz, size_t cap, heap_node_t node)
{
    if (*sz >= cap) return;               /* safety, should not happen */
    h[*sz] = node;
    size_t i = *sz;
    (*sz)++;

    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (cmp_seq(&h[i].val, &h[parent].val) < 0) {
            heap_node_t tmp = h[i];
            h[i] = h[parent];
            h[parent] = tmp;
            i = parent;
        } else break;
    }
}

static heap_node_t heap_pop(heap_node_t *h, size_t *sz)
{
    heap_node_t root = h[0];
    (*sz)--;
    if (*sz == 0) return root;
    h[0] = h[*sz];

    size_t i = 0;
    while (1) {
        size_t left = 2 * i + 1;
        if (left >= *sz) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < *sz && cmp_seq(&h[right].val, &h[left].val) < 0)
            smallest = right;
        if (cmp_seq(&h[smallest].val, &h[i].val) < 0) {
            heap_node_t tmp = h[i];
            h[i] = h[smallest];
            h[smallest] = tmp;
            i = smallest;
        } else break;
    }
    return root;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for offset 1 */

    /* Choose an upper bound for e; grows with n. */
    idx_t e_limit = n * 20 + 100;
    size_t cap = (size_t)e_limit;

    heap_node_t *heap = (heap_node_t *)malloc(cap * sizeof(heap_node_t));
    if (!heap) exit(1);
    size_t sz = 0;

    /* Initialize heap with the first pair from each e-stream. */
    for (idx_t e = 0; e < e_limit; ++e) {
        idx_t o_start = (e == 0) ? 1 : 0;      /* ensure e+o > 0 */
        seq_t val;
        if (concat_fits(&val, e, o_start))
            heap_push(heap, &sz, cap,
                      (heap_node_t){val, e, o_start});
    }

    seq_t prev_val = 0;
    bool have_prev = false;
    seq_t result   = 0;
    idx_t count    = 0;

    while (count < n) {
        heap_node_t cur = heap_pop(heap, &sz);

        /* Count distinct values only. */
        if (!have_prev || cmp_seq(&cur.val, &prev_val) != 0) {
            count++;
            result   = cur.val;
            prev_val = cur.val;
            have_prev = true;
            if (count == n) break;
        }

        /* Advance this stream to the next o. */
        seq_t next_val;
        if (concat_fits(&next_val, cur.e, cur.o + 1))
            heap_push(heap, &sz, cap,
                      (heap_node_t){next_val, cur.e, cur.o + 1});
    }

    free(heap);
    return result;
}
