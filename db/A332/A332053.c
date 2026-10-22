/*
 * A332053  a(n) is the number of sets modulo n which can be formed by a
 *          finite arithmetic sequence, whose complement cannot be formed by a
 *          finite arithmetic sequence.
 * https://oeis.org/A332053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ( sigma(n) - tau(n) - n + (n mod 2) ) for n > 2 and
 *            a(n) = 0 for n = 1, 2. sigma(n) is the sum of divisors, tau(n)
 *            is the number of divisors of n. The function implements a direct
 *            factorisation by trial division, which is fast enough for n up
 *            to at least 10 000. The computation is exac...
 * Range:     u8 n <= 17, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 2 the definition gives 0. */
    if (n <= 2) {
        return 0;
    }

    /* Work with unsigned 64‑bit integers for the trial‑division loop. */
    idx_t t = n;                  /* mutable remainder of n */
    seq_t sigma = 1;              /* product of prime‑power sigma factors */
    seq_t tau   = 1;              /* product of (exp+1) for each prime */

    /* Trial division: examine all primes up to sqrt(t).  */
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int exp = 0;
            seq_t p_pow = 1;      /* p^exp */
            while (t % p == 0) {
                t /= p;
                ++exp;
                p_pow *= (seq_t)p;
            }
            /* sigma factor for the prime power: (p^(exp+1)-1)/(p-1) */
            seq_t term = (p_pow * (seq_t)p - 1) / ((seq_t)p - 1);
            sigma *= term;
            tau   *= (seq_t)(exp + 1);
        }
    }

    /* If a prime factor larger than sqrt(original n) remains. */
    if (t > 1) {
        seq_t term = ((seq_t)t * (seq_t)t - 1) / ((seq_t)t - 1); /* e = 1 */
        sigma *= term;
        tau   *= 2;
    }

    /* Final formula: a(n) = n * (sigma - tau - n + parity). */
    seq_t n_seq   = (seq_t)n;
    seq_t parity  = (seq_t)(n % 2);
    seq_t result  = n_seq * (sigma - tau - n_seq + parity);

    return result;
}
