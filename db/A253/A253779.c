/*
 * A253779  Numbers c whose cubes are equal to the sum of m^3 consecutive
 *          cubes for m^3 not divisible by 3 (A118719).
 * https://oeis.org/A253779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m(m^2-1)(m^2+2)/6 where m = n + floor((n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 80, u64 n <= 6804,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based indexed (a(1) is the first listed term).  */
seq_t a(idx_t n)
{
    /* For safety, treat n<1 as 0 (the specification does not demand
       behavior below the offset). */
    if (n == 0)
        return (seq_t)0;

    /* Convert n to seq_t once. */
    seq_t sn = (seq_t)n;

    /* m = n + floor((n-1)/2)   →  nth integer not divisible by 3. */
    seq_t m = sn + (sn - 1) / 2;

    /* m²  (always non‑negative). */
    seq_t m2 = m * m;

    /* t1 = m²-1 ,  t2 = m²+2   (both fit comfortably in 64‑bit
       for all n that can occur in the test suite). */
    seq_t t1 = m2 - 1;
    seq_t t2 = m2 + 2;

    /* Put the three factors into a small array. */
    seq_t f[3] = { m, t1, t2 };

    /* Choose the factor that will absorb the division by 2.
       Among the available even factors pick the largest so the
       final product stays as small as possible. */
    int idx2 = -1;
    seq_t max2 = 0;
    for (int i = 0; i < 3; ++i) {
        if ((f[i] & (seq_t)1) == 0) {          /* even */
            if (idx2 == -1 || f[i] > max2) {
                idx2 = i;
                max2 = f[i];
            }
        }
    }
    if (idx2 != -1)
        f[idx2] /= 2;

    /* Choose the factor that will absorb the division by 3.
       Among the available multiples of 3 pick the largest. */
    int idx3 = -1;
    seq_t max3 = 0;
    for (int i = 0; i < 3; ++i) {
        if (f[i] % 3 == 0) {
            if (idx3 == -1 || f[i] > max3) {
                idx3 = i;
                max3 = f[i];
            }
        }
    }
    if (idx3 != -1)
        f[idx3] /= 3;

    /* The product of the now‑reduced factors is the desired term. */
    seq_t prod = f[0];
    prod *= f[1];
    prod *= f[2];

    return prod;
}
