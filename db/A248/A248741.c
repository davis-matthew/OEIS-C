/*
 * A248741  Nonnegative integers of the form m^k/k where k > 1.
 * https://oeis.org/A248741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, prime_factor_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A248741: nonnegative integers of the form m^k/k (k>1),
   sorted increasingly and with duplicates removed.  The first term
   a(1) is 0 (0^2/2). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/prime_factor_product.h"

#define KMAX 12   /* enough to generate the first 10 000 terms */

typedef struct {
    seq_t val;   /* (m^k)/k */
    idx_t k;     /* exponent */
    idx_t m;     /* current base (must be multiple of step) */
    idx_t step;  /* product of distinct primes dividing k */
} Node;

/* ---------- binary min‑heap of Nodes (fixed size) ---------- */
static Node heap[KMAX];
static size_t heap_sz = 0;

static void heap_push(Node x) {
    size_t i = heap_sz++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= x.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = x;
}

static Node heap_pop(void) {
    Node res = heap[0];
    Node last = heap[--heap_sz];
    size_t i = 0, n = heap_sz;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= n) break;
        size_t r = l + 1;
        size_t child = (r < n && heap[r].val < heap[l].val) ? r : l;
        if (heap[child].val >= last.val) break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return res;
}

/* ---------- main function ---------- */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    /* initialise heap with the first valid value for each k (k≥2) */
    heap_sz = 0;
    for (idx_t k = 2; k <= KMAX; ++k) {
        idx_t step = prime_factor_product(k);   /* minimal m divisor */
        seq_t val = big_pow((seq_t)step, (seq_t)k);
        val /= (seq_t)k;                         /* (step^k)/k */
        heap_push((Node){.val = val, .k = k, .m = step, .step = step});
    }

    idx_t count = 1;                 /* a(1)=0 already counted */
    seq_t prev_val = 0;

    while (count < n) {
        Node cur = heap[0];

        /* skip duplicates */
        if (cur.val == prev_val) {
            heap_pop();
            cur.m += cur.step;                     /* advance to next base */
            cur.val = big_pow((seq_t)cur.m, (seq_t)cur.k);
            cur.val /= (seq_t)cur.k;
            heap_push(cur);
            continue;
        }

        /* new unique value */
        heap_pop();
        prev_val = cur.val;
        ++count;
        if (count == n) return prev_val;

        /* advance this k to the next base and push back */
        cur.m += cur.step;
        cur.val = big_pow((seq_t)cur.m, (seq_t)cur.k);
        cur.val /= (seq_t)cur.k;
        heap_push(cur);
    }

    /* should never reach here */
    return 0;
}
