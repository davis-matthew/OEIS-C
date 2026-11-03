/*
 * A337624  a(n) is the least positive integer in base n that when multiplied
 *          by 2n-1 will contain only the digits 0 and 1.
 * https://oeis.org/A337624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A337623(n)/(2*n-1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A337623(n) is the least positive integer in base n
 *   using only the digits 0 and 1 that is divisible by (2*n-1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute a single term; assumes n >= 1 and cache for this index is ready. */
static seq_t compute_single(idx_t n)
{
    if (n == 1) return 1;                     /* base case */

    /* Modulus is d = 2*n-1. */
    idx_t d64 = 2 * n - 1;
    int d = (int)d64;                         /* fits comfortably because n <= 10000 */

    /* Breadth‑first search over residues modulo d. */
    int *parent = (int *)malloc(d * sizeof(int));
    uint8_t *digit = (uint8_t *)malloc(d * sizeof(uint8_t));
    bool *visited = (bool *)malloc(d * sizeof(bool));
    int *queue = (int *)malloc(d * sizeof(int));
    if (!parent || !digit || !visited || !queue) abort();

    for (int i = 0; i < d; ++i) {
        visited[i] = false;
        parent[i] = -1;
    }

    int head = 0, tail = 0;
    visited[1 % d] = true;
    parent[1 % d] = 0;                /* root has no parent */
    digit[1 % d] = 1;                 /* first digit is always '1' */
    queue[tail++] = 1 % d;

    bool found = false;

    while (head < tail && !found) {
        int r = queue[head++];

        int next0 = (int)(((uint64_t)r * (uint64_t)n) % (uint64_t)d);
        if (!visited[next0]) {
            visited[next0] = true;
            parent[next0] = r;
            digit[next0] = 0;
            queue[tail++] = next0;
            if (next0 == 0) { found = true; break; }
        }

        int next1 = (int)(((uint64_t)r * (uint64_t)n + 1ULL) % (uint64_t)d);
        if (!visited[next1]) {
            visited[next1] = true;
            parent[next1] = r;
            digit[next1] = 1;
            queue[tail++] = next1;
            if (next1 == 0) { found = true; break; }
        }
    }

    /* Reconstruct the digit sequence from the residue 0 back to the root.
       The digits are collected in reverse order. */
    int rev_cap = d;
    uint8_t *rev = (uint8_t *)malloc(rev_cap * sizeof(uint8_t));
    int rev_len = 0;

    /* Start from residue 0; include digit[0], then move to parent until
       we reach 0 again (root's parent). */
    int cur = 0;
    rev[rev_len++] = digit[cur];     /* digit that produced residue 0 */
    cur = parent[cur];
    while (cur != 0) {
        rev[rev_len++] = digit[cur];
        cur = parent[cur];
    }

    /* Compute the full base‑n number B represented by the 0/1 digits. */
    seq_t B = 0;
    for (int i = rev_len - 1; i >= 0; --i) {
        B = B * (seq_t)n + (seq_t)rev[i];
    }

    seq_t result = B / (seq_t)d64;   /* B is divisible by d64 */

    /* Clean up */
    free(parent);
    free(digit);
    free(visited);
    free(queue);
    free(rev);

    return result;
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    size_t new_size = (size_t)n + 1;          /* indices 0..n; we use 1..n */
    seq_t *new_cache = mem_resize(seq_t, cache, new_size);
    if (!new_cache) abort();                  /* allocation failure */
    /* initialise newly added entries (indices cache_size+1 .. n) */
    for (idx_t i = cache_size + 1; i < new_size; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n=0, safe guard */
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];  /* memoised result */
    cache[n] = compute_single(n);
    return cache[n];
}
