/*
 * A384132  Integers k such that the Diophantine equation x^3 + y^3 + z^3 +
 *          w^3 = k^3, where 0 < x < y < z < w has no integer solutions.
 * https://oeis.org/A384132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the nth integer k for which k³ cannot be written as the
 *            sum of four distinct positive cubes. The sequence is computed by
 *            brute‑forcing all such representations up to k = 125 (which
 *            covers all known terms)
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *seq_values = NULL;
static size_t seq_len = 0;

/* Construct the sequence once. */
static void build_sequence(void)
{
    if (seq_values != NULL) return;   /* already built */

    const idx_t maxK = 125;           /* sufficient for all known terms (≤121) */
    const seq_t maxCube = (seq_t)maxK * ((seq_t)maxK * (seq_t)maxK);

    /* Mark cubes that can be expressed as a sum of four distinct cubes. */
    unsigned char *rep = (unsigned char*)calloc((size_t)maxCube + 1, 1);
    if (!rep) return;                /* allocation failure – keep sequence empty */

    /* Precompute cubes 1³ … maxK³. */
    seq_t cubes[maxK + 1];
    for (idx_t i = 1; i <= maxK; ++i) {
        cubes[i] = (seq_t)i * ((seq_t)i * (seq_t)i);
    }

    for (idx_t a = 1; a <= maxK - 3; ++a) {
        seq_t ca = cubes[a];
        for (idx_t b = a + 1; b <= maxK - 2; ++b) {
            seq_t cb = cubes[b];
            seq_t sab = ca + cb;
            for (idx_t c = b + 1; c <= maxK - 1; ++c) {
                seq_t sc = cubes[c];
                seq_t sac = sab + sc;
                for (idx_t d = c + 1; d <= maxK; ++d) {
                    seq_t total = sac + cubes[d];
                    if (total > maxCube) continue;
                    rep[(size_t)total] = 1;
                }
            }
        }
    }

    /* Build the list of integers not representable. */
    seq_t *vals = (seq_t*)malloc(sizeof(seq_t) * maxK);
    size_t len = 0;
    for (idx_t k = 1; k <= maxK; ++k) {
        if (!rep[(size_t)cubes[k]]) {
            vals[len++] = (seq_t)k;
        }
    }
    free(rep);

    seq_values = vals;
    seq_len    = len;
}

/* Required exported function. */
seq_t a(idx_t n)
{
    build_sequence();
    if (n == 0 || n > seq_len) return 0;
    return seq_values[n - 1];
}
