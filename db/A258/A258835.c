/*
 * A258835  Expansion of psi(x)^3 * psi(x^4) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A258835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(8*n + 7) / 8
 * Range:     u8 n <= 24, u16 n <= 7608, u32 n <= 9266, u64 n <= 9402,
 *            u128 n <= 9402, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma is the sum of divisors function. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static int *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_built = false;

/* simple power function: base^exp, where exp is small (≤ 10) */
static seq_t small_pow(seq_t base, int exp) {
    seq_t res = 1;
    while (exp--)
        res *= base;
    return res;
}

/* Build a small prime table up to LIMIT.  This is done once. */
static void ensure_primes(void) {
    if (primes_built) return;
    const int LIMIT = 100000; /* > sqrt(8*max_n+7) for all reasonable n */
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    int *temp = (int*)malloc((LIMIT + 1) * sizeof(int));
    size_t cnt = 0;

    for (int i = 2; i <= LIMIT; i++) {
        if (!is_comp[i]) {
            temp[cnt++] = i;
            if ((long long)i * i <= LIMIT) {
                for (long long j = (long long)i * i; j <= LIMIT; j += i)
                    is_comp[(int)j] = true;
            }
        }
    }

    prime_list = temp;
    prime_count = cnt;
    free(is_comp);
    primes_built = true;
}

seq_t a(idx_t n) {
    ensure_primes();

    idx_t m = n * 8 + 7;          /* target number */
    seq_t sigma = 1;
    idx_t temp = m;

    for (size_t i = 0; i < prime_count && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= temp; ++i) {
        int p = prime_list[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }

            seq_t p_seq = (seq_t)p;
            /* term = (p^(exp+1)-1)/(p-1) */
            seq_t numerator   = small_pow(p_seq, exp + 1) - 1;
            seq_t denominator = p_seq - 1;
            seq_t factor = numerator / denominator;
            sigma *= factor;
        }
    }

    if (temp > 1) {   /* remaining prime factor */
        seq_t p_seq = temp;
        seq_t factor = (p_seq * p_seq - 1) / (p_seq - 1); /* = p + 1 */
        sigma *= factor;
    }

    return sigma / 8;
}
