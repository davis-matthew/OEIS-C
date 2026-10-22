/*
 * A318952  a(n) = Sum_{d|n} (sigma(n) mod sigma(d)).
 * https://oeis.org/A318952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} (sigma(n) mod sigma(d))
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sigma(p^e) iteratively and stores in sigma_pp array. */
static seq_t sigma_prime_power(uint64_t p, int e)
{
    seq_t sum = (seq_t)1;
    seq_t pow = (seq_t)1;
    for (int i = 1; i <= e; ++i) {
        pow *= (seq_t)p;
        sum += pow;
    }
    return sum;
}

/* Recursive enumeration of all divisors, accumulating the desired sum. */
static void enumerate_divisors(int idx, seq_t sigma_d,
                               const seq_t sigma_pp[][64],
                               const int exps[],
                               int cnt,
                               seq_t sigma_n,
                               seq_t *res)
{
    if (idx == cnt) {
        *res += sigma_n % sigma_d;
        return;
    }

    for (int e = 0; e <= exps[idx]; ++e) {
        seq_t next_sigma_d = sigma_d * sigma_pp[idx][e];
        enumerate_divisors(idx + 1, next_sigma_d, sigma_pp, exps, cnt, sigma_n, res);
    }
}

seq_t a(idx_t n)
{
    /* Preprocessing: factorization of n into primes & exponents */
    uint64_t primes[64];
    int     exps[64];
    int     cnt = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt]   = p;
            exps[cnt]     = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt]   = temp;
        exps[cnt]     = 1;
        ++cnt;
    }

    /* Precompute sigma(p^e) for each prime factor */
    seq_t sigma_pp[64][64];
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        sigma_pp[i][0] = (seq_t)1;
        seq_t pow = (seq_t)1;
        for (int e = 1; e <= exps[i]; ++e) {
            pow *= (seq_t)p;
            sigma_pp[i][e] = sigma_pp[i][e - 1] + pow;
        }
    }

    /* Compute sigma(n) */
    seq_t sigma_n = (seq_t)1;
    for (int i = 0; i < cnt; ++i) {
        sigma_n *= sigma_pp[i][exps[i]];
    }

    /* Enumerate divisors and sum sigma(n) mod sigma(d) */
    seq_t result = (seq_t)0;
    enumerate_divisors(0, (seq_t)1, sigma_pp, exps, cnt, sigma_n, &result);

    return result;
}
