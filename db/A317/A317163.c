/*
 * A317163  a(n) = 48277590120607451 + (n-1)*8440735245322380.
 * https://oeis.org/A317163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48277590120607451 + (n-1)*8440735245322380
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset 1, the behavior is undefined;
       returning the first term keeps the function well‑defined. */
    if (n == 0)
        return (seq_t)48277590120607451ULL;

    const seq_t base = (seq_t)48277590120607451ULL;
    const seq_t diff = (seq_t)8440735245322380ULL;
    return base + diff * (seq_t)(n - 1);
}
