/*
 * A354848  a(n) = (n-1)! * Sum_{d|n} d^(n/d + 1).
 * https://oeis.org/A354848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{d|n} d^(n/d + 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast integer power for seq_t values. */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1) result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* Recursively enumerate all divisors of n using its prime factorisation
   and accumulate the required sum. */
static void rec_div(idx_t n, const idx_t *primes, const int *exps, int idx,
                    int num, idx_t cur, seq_t *sum) {
    if (idx == num) {
        idx_t d = cur;
        idx_t e = n / d + 1;
        *sum += pow_seq_t((seq_t)d, e);
        return;
    }
    idx_t p = primes[idx];
    idx_t mult = 1;
    for (int k = 0; k <= exps[idx]; ++k) {
        rec_div(n, primes, exps, idx + 1, num, cur * mult, sum);
        mult *= p;
    }
}

seq_t a(idx_t n) {
    /* (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) fact *= (seq_t)i;

    /* Prime factorisation of n */
    idx_t temp = n;
    idx_t primes[16];    /* 16 is enough for n <= 10000 */
    int exps[16];
    int num = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) { temp /= p; ++cnt; }
            primes[num] = p;
            exps[num] = cnt;
            ++num;
        }
    }
    if (temp > 1) {
        primes[num] = temp;
        exps[num] = 1;
        ++num;
    }

    /* Sum over divisors */
    seq_t sum = 0;
    rec_div(n, primes, exps, 0, num, 1, &sum);

    /* a(n) = (n-1)! * Σ_{d|n} d^(n/d + 1) */
    return fact * sum;
}
