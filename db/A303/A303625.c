/*
 * A303625  Number of 3Xn 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) -9*a(n-2) -54*a(n-3) +79*a(n-4) +193*a(n-5)
 *            -246*a(n-6) -383*a(n-7) +307*a(n-8) +547*a(n-9) -250*a(n-10)
 *            -458*a(n-11) +174*a(n-12) +193*a(n-13) -66*a(n-14) -34*a(n-15)
 *            -3*a(n-16), n > 17 Initial values (n = 1 … 17): 4, 2, 38, 68,
 *            362, 1283, 5411, 22516, 96189, 417555, 1829258, 809...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    if (n < 1U)          /* offset starts at 1 */
        return (seq_t)0;

    /* Base terms (1‑based).  Index 0 is unused. */
    static const seq_t init[18] = {
        0ULL,
        (seq_t)4ULL,
        (seq_t)2ULL,
        (seq_t)38ULL,
        (seq_t)68ULL,
        (seq_t)362ULL,
        (seq_t)1283ULL,
        (seq_t)5411ULL,
        (seq_t)22516ULL,
        (seq_t)96189ULL,
        (seq_t)417555ULL,
        (seq_t)1829258ULL,
        (seq_t)8090336ULL,
        (seq_t)35972626ULL,
        (seq_t)160686336ULL,
        (seq_t)719920275ULL,
        (seq_t)3232692431ULL,
        (seq_t)14538525542ULL
    };

    if (n <= 17U)
        return init[n];

    /* Buffer holding the last 17 terms: buf[0]=a(k-16)…buf[16]=a(k)   */
    seq_t buf[17];
    for (size_t i = 0; i < 17; ++i)
        buf[i] = init[i + 1];

    for (idx_t k = 18U; k <= n; ++k) {
        seq_t val =
            (seq_t)8  * buf[16]
          - (seq_t)9  * buf[15]
          - (seq_t)54 * buf[14]
          + (seq_t)79 * buf[13]
          + (seq_t)193 * buf[12]
          - (seq_t)246 * buf[11]
          - (seq_t)383 * buf[10]
          + (seq_t)307 * buf[9]
          + (seq_t)547 * buf[8]
          - (seq_t)250 * buf[7]
          - (seq_t)458 * buf[6]
          + (seq_t)174 * buf[5]
          + (seq_t)193 * buf[4]
          - (seq_t)66  * buf[3]
          - (seq_t)34  * buf[2]
          - (seq_t)3   * buf[1];

        /* shift left so that buf[0] becomes a(k-15) etc. */
        for (size_t i = 0; i < 16; ++i)
            buf[i] = buf[i + 1];
        buf[16] = val;
    }

    return buf[16];
}
