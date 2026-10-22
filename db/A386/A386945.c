/*
 * A386945  Short legs of nonprimitive Pythagorean triples of the form (p^2 -
 *          q^2, 2*p*q, p^2 + q^2), ordered by increasing hypotenuse
 *          (A386943).
 * https://oeis.org/A386945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 8226, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386945: short legs of nonprimitive Pythagorean triples
   of the form (p^2−q^2, 2pq, p^2+q^2), ordered by increasing hypotenuse.
   For triples with the same hypotenuse, the short legs are ordered
   descending (larger first). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* helper structure for a triple */
typedef struct {
    seq_t c;   /* hypotenuse */
    seq_t s;   /* short leg */
} Triple;

/* dynamical array of triples */
static Triple* triples = NULL;
static size_t  triples_cap = 0;
static size_t  triples_len  = 0;
static idx_t   current_limit_c = 0;   /* maximum hypotenuse seen */

/* Euclid's GCD for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

/* test if a pair (p,q) gives a non‑primitive triple */
static inline bool is_nonprimitive(idx_t p, idx_t q)
{
    return (gcd_idx(p, q) > 1) || ((p & 1ULL) && (q & 1ULL));
}

/* comparison for qsort: by hypotenuse ascending,
   if equal hypotenuse, by short leg descending. */
static int triple_cmp(const void *p1, const void *p2)
{
    const Triple *t1 = (const Triple *)p1,
                 *t2 = (const Triple *)p2;
    if (t1->c < t2->c) return -1;
    if (t1->c > t2->c) return  1;
    /* same hypotenuse: larger short leg first */
    if (t1->s > t2->s) return -1;
    if (t1->s < t2->s) return  1;
    return 0;
}

/* generate all non‑primitive triples with hypotenuse in (old_limit_c, limit_c] */
static void generate_up_to(idx_t limit_c)
{
    if (limit_c <= current_limit_c) return;

    const idx_t old_limit_c = current_limit_c;
    current_limit_c = limit_c;

    for (idx_t p = 1; p * p <= limit_c; ++p) {
        seq_t p2 = (seq_t)p * (seq_t)p;
        for (idx_t q = 1; q < p; ++q) {
            seq_t q2 = (seq_t)q * (seq_t)q;
            seq_t c  = p2 + q2;
            if (c <= limit_c && c > old_limit_c) {
                if (!is_nonprimitive(p, q)) continue;
                if (triples_len == triples_cap) {
                    const size_t new_cap = triples_cap ? triples_cap * 2 : 64;
                    Triple *tmp = (Triple *)realloc(triples, new_cap * sizeof(Triple));
                    if (!tmp) exit(EXIT_FAILURE);
                    triples = tmp;
                    triples_cap = new_cap;
                }
                seq_t a_ = p2 - q2;
                seq_t b_ = (seq_t)2 * (seq_t)p * (seq_t)q;
                triples[triples_len].c = c;
                triples[triples_len].s = (a_ < b_) ? a_ : b_;
                ++triples_len;
            }
        }
    }

    /* keep the collection sorted */
    if (triples_len > 1)
        qsort(triples, triples_len, sizeof(Triple), triple_cmp);
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe for n>=1 */

    /* generate enough triples */
    while (triples_len < (size_t)n) {
        const idx_t new_limit = current_limit_c ? current_limit_c * 2 : 16;
        generate_up_to(new_limit);
    }

    /* triples are sorted by hypotenuse, return the short leg of the n‑th item */
    return triples[n-1].s;
}
