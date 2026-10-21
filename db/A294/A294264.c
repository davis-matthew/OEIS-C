/*
 * A294264  a(n) = (5/128)*n^4*(n mod 2) + (((-5/128)*n^4*(n mod 2) - 26) mod
 *          n) + n^3 (n > 0).
 * https://oeis.org/A294264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5/128)*n^4*(n mod 2) + (((-5/128)*n^4*(n mod 2) - 26)
 *            mod n) + n^3 (n > 0)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 570, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, safety */

    /* use signed 128‑bit to avoid overflow and handle negatives */
    __int128 N = ( __int128 )n;     /* cast from unsigned index */
    __int128 M = N * N * N * N;     /* n^4 */

    if ((n & 1u) == 0) M = 0;       /* factor (n mod 2) */

    /* ceil(5*M/128) using integer arithmetic */
    __int128 term1 = (5 * M + 127) / 128;   /* ceil division */

    /* floor(-5*M/128) = -ceil(5*M/128) = -term1 */
    __int128 q = -term1;

    /* q - 26, then positive modulo n */
    __int128 r = q - 26;
    __int128 mod = ((r % N) + N) % N;      /* make remainder non‑negative */

    __int128 N3 = N * N * N;       /* n^3 */

    __int128 result = term1 + mod + N3;
    return (seq_t) result;
}
