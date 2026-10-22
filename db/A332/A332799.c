/*
 * A332799  Numbers whose smallest prime factor is 17.
 * https://oeis.org/A332799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*A008366(n). Numbers whose smallest prime factor is 17
 * Range:     u8 n <= 1, u16 n <= 740, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the n‑th integer coprime to 2·3·5·7·11·13 (i.e. 30030),
   then multiply by 17.  The coprime values repeat every 30030 numbers, with
   5760 coprimes per block (phi(30030)).  We pre‑compute one block once. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants */
    static const idx_t BLOCK_UPPER  = 30030;   /* 2*3*5*7*11*13 */
    static const size_t  PHI          = 5760;   /* φ(30030) */
    static const seq_t  MULTIPLIER    = ((seq_t)17) * ((seq_t)BLOCK_UPPER); /* 17*30030 */

    /* static buffer for the first block of coprimes, built on first call */
    static idx_t *coprimes = NULL;
    if (!coprimes) {
        /* The block contains exactly PHI (=5760) coprime integers. */
        coprimes = (idx_t *)malloc(PHI * sizeof(idx_t));
        size_t pos = 0;
        for (idx_t i = 1; i <= BLOCK_UPPER; ++i) {
            if ((i & 1) && (i % 3) && (i % 5) &&
                (i % 7) && (i % 11) && (i % 13)) {
                coprimes[pos++] = i;
            }
        }
    }

    /* Determine which block and position within the block */
    idx_t block = (n - 1) / PHI;        /* zero‑based block index */
    idx_t idx   = (n - 1) % PHI;        /* zero‑based index inside the block */

    /* Value of the n‑th coprime integer */
    seq_t base   = ((seq_t)block) * ((seq_t)BLOCK_UPPER)
                   + ((seq_t)coprimes[idx]);

    /* Final sequence value: 17 times the coprime integer. */
    return ((seq_t)17) * base;
}
