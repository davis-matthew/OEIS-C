/*
 * A343810  Numbers that contain only the digits 0,4,8.
 * https://oeis.org/A343810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained by writing (n-1) in base‑3 and
 *            replacing each digit 0,1,2 with 0,4,8 respectively
 * Range:     u8 n <= 9, u16 n <= 162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset is 1, undefined for n==0 */

    idx_t k = n - 1;                /* integer to convert */
    seq_t result    = 0;            /* accumulated decimal value */
    seq_t multiplier = 1;           /* 10^position, starting at ones place */

    while (k != 0) {
        unsigned digit = (unsigned)(k % 3);   /* current base‑3 digit */
        seq_t mapped    = (seq_t)(digit * 4U); /* 0 → 0, 1 → 4, 2 → 8 */
        result += mapped * multiplier;        /* add current decimal digit */

        multiplier *= (seq_t)10;               /* move to next decimal position */
        k      /= 3;                           /* progress in base‑3 */
    }

    return result;
}
