/*
 * A260911  Least positive integer k < prime(n) such that there are 0 < i < j
 *          < k for which i^2 + j^2 = k^2 and i,j,k are all quadratic residues
 *          modulo prime(n), or 0 if no such k exists.
 * https://oeis.org/A260911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint32, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A260911:  least positive k<p where k<prime(n)
   and some i<j<k with i^2+j^2=k^2 and i,j,k are non‑zero quadratic
   residues modulo prime(n).  The result is returned as a seq_t and
   0 if no such k exists.  The routine caches Pythagorean triples
   up to the largest prime requested. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/gcd_uint32.h"

/*---------------------------------------------------------------------*/
/*  Data structures and static state for caching triangles           */
/*---------------------------------------------------------------------*/
typedef struct {
    uint32_t i;   /* first leg  (i < j < k) */
    uint32_t j;   /* second leg (i < j < k) */
    uint32_t k;   /* hypotenuse (i < j < k) */
} triple_t;

static triple_t *triples   = NULL;   /* array of stored triples            */
static size_t    triples_sz = 0;     /* number of stored triples           */
static size_t    triples_cap = 0;    /* allocated capacity                  */
static uint32_t  triples_lim = 0;    /* largest prime (exclusive) handled   */

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/*  Compare triples by ascending hypotenuse (k) ----------------------*/
static int cmp_triple(const void *x, const void *y)
{
    const triple_t *tx = (const triple_t *)x;
    const triple_t *ty = (const triple_t *)y;
    if (tx->k < ty->k) return -1;
    if (tx->k > ty->k) return 1;
    return 0;
}

/*---------------------------------------------------------------------*/
/*  Generate all Pythagorean triples with hypotenuse < limit  ---------*/
static void generate_triples(uint32_t limit)
{
    free(triples);
    triples = NULL;
    triples_sz = 0;
    triples_cap = 0;
    triples_lim = limit;

    for (uint32_t m = 2; ; ++m) {
        if ((uint64_t)m * m >= limit) break;
        for (uint32_t n = 1; n < m; ++n) {
            if ( ((m - n) & 1) == 0 ) continue;     /* same parity   */
            if ( gcd_uint32(m, n) != 1 ) continue;   /* not coprime   */

            uint32_t a  = m*m - n*n;
            uint32_t b  = 2*m*n;
            uint32_t c  = m*m + n*n;

            /* order the legs so i<j  */
            uint32_t i = a, j = b;
            if (i > j) { uint32_t t = i; i = j; j = t; }

            for (uint32_t d = 1; ; ++d) {
                uint64_t k64 = (uint64_t)c * d;
                if (k64 >= limit) break;
                uint32_t k = (uint32_t)k64;
                uint32_t i_scaled = i * d;
                uint32_t j_scaled = j * d;

                if (triples_sz == triples_cap) {
                    size_t new_cap = triples_cap ? triples_cap * 2 : 16;
                    triple_t *tmp = (triple_t *)realloc(triples, new_cap * sizeof(triple_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    triples = tmp;
                    triples_cap = new_cap;
                }
                triples[triples_sz].i = i_scaled;
                triples[triples_sz].j = j_scaled;
                triples[triples_sz].k = k;
                ++triples_sz;
            }
        }
    }

    /* Sort by hypotenuse to allow early termination */
    qsort(triples, triples_sz, sizeof(triple_t), cmp_triple);
}

/*---------------------------------------------------------------------*/
/*  Main sequence function  ------------------------------------------*/
seq_t a(idx_t n)
{
    /* obtain the n-th prime */
    uint32_t p = get_prime((idx_t)n);

    /* regenerate triples if required */
    if (p > triples_lim)
        generate_triples(p);

    /* Build quadratic‑residue table modulo p  */
    /*   res[r] == 1  iff r is a non‑zero quadratic residue mod p   */
    uint8_t *res = (uint8_t *)calloc((size_t)p, sizeof(uint8_t));
    if (!res) exit(EXIT_FAILURE);

    for (uint32_t x = 1; x < p; ++x) {
        uint32_t r = (uint32_t)(((uint64_t)x * x) % p);
        res[r] = 1;
    }

    /* Scan precomputed triples for the smallest qualifying k */
    for (size_t idx = 0; idx < triples_sz; ++idx) {
        uint32_t k = triples[idx].k;
        if (k >= p) break;            /* beyond the prime */
        if (res[triples[idx].i] &&
            res[triples[idx].j] &&
            res[k]) {
            free(res);
            return (seq_t)k;
        }
    }

    free(res);
    return 0;
}
