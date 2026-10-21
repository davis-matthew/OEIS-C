/*
 * A262478  a(n) = Sum_{i >= 0} d_i(n) * p_(i + 1) where d_i(n) = i-th digit
 *          of n in base 3, and p_i = i-th prime.
 * https://oeis.org/A262478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} d_i(n) * p_{i+1}, * where d_i(n) is the i‑th
 *            base‑3 digit of n (least significant first) * and p_i is the
 *            i‑th prime (2, 3, 5, 7, …)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First 100 primes – sufficient for any 64‑bit n (≤ 41 base‑3 digits). */
    static const seq_t primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
        73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
        157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,
        241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,
        347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,
        439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,
        547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,
        643,647,653
    };

    seq_t sum = 0;
    idx_t m = n;
    idx_t i = 0;

    while (m > 0)
    {
        seq_t digit = (seq_t)(m % 3);
        if (digit)          /* skip zero digits – saves a few adds */
            sum += digit * primes[i];
        m /= 3;
        ++i;
    }
    return sum;
}
