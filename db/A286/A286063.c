/*
 * A286063  Fixed point of the mapping 00->001, 1->100, starting with 00.
 * https://oeis.org/A286063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if (parity of (n-1)/3 == 0 and (n-1)%3 == 2) or (parity
 *            of (n-1)/3 == 1 and (n-1)%3 == 0); otherwise 0. Here parity is
 *            popcount modulo 2. The sequence is the fixed point of the
 *            mapping 00 → 001, 1 → 100, started with 00
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based. */
    idx_t idx0 = n - 1;                      /* 0‑based index */
    idx_t block = idx0 / 3;                  /* index of the block (00 or 1) */
    unsigned int pos = (unsigned int)(idx0 % 3);  /* position inside the block */

    /* parity of popcount(block) */
    unsigned int parity = (__builtin_popcountll(block) & 1U);

    /* A (00) → 001,  B (1) → 100 */
    if (parity == 0) {           /* block = 00 → 001 */
        return (pos == 2) ? (seq_t)1 : (seq_t)0;
    } else {                     /* block = 1  → 100 */
        return (pos == 0) ? (seq_t)1 : (seq_t)0;
    }
}
