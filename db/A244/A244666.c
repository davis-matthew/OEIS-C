/*
 * A244666  Numbers n such that floor(antisigma(n) / sigma(n)) =
 *          floor(antisigma(n+1) / sigma(n+1)).
 * https://oeis.org/A244666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer such that * floor((n*(n+1)/2)/sigma(n))
 *            = floor(((n+1)*(n+2)/2)/sigma(n+1)) * where sigma(n) is the sum
 *            of all positive divisors of n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Local routine to generate primes up to 1,000,000. */
static uint32_t primes[80000];
static size_t prime_cnt = 0;

static void init_primes(void) {
    static bool done = false;
    if (done) return;
    const int LIMIT = 1000000;
    bool *is_comp = (bool*)calloc(LIMIT + 1, 1);
    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    done = true;
}

/* Compute sigma(n) by trial division using the primes list. */
static seq_t sigma_u64(uint64_t n) {
    seq_t sum = 1;
    uint64_t temp = n;
    init_primes();

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            seq_t subtotal = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                subtotal += power;
            }
            sum *= subtotal;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        sum *= (seq_t)(1 + temp);
    }
    return sum;
}

/* Cache for found sequence terms. */
static seq_t *terms = NULL;
static size_t len   = 0;
static size_t cap   = 0;
static idx_t max_checked = 0;

/* Append a value to the terms array, expanding capacity as needed. */
static void append_term(seq_t val) {
    if (len == cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        cap   = new_cap;
    }
    terms[len++] = val;
}

/* Main sequence routine. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1, behaviour for n<1 undefined */

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        idx_t m = max_checked + 1;

        seq_t sigma_m  = sigma_u64((uint64_t)m);
        seq_t sigma_mp = sigma_u64((uint64_t)(m + 1));

        seq_t tn   = ((seq_t)m * (seq_t)(m + 1)) / 2;
        seq_t tn1  = ((seq_t)(m + 1) * (seq_t)(m + 2)) / 2;

        seq_t f1 = tn / sigma_m;
        seq_t f2 = tn1 / sigma_mp;

        if (f1 == f2)
            append_term((seq_t)m);

        max_checked = m;
    }

    return terms[(size_t)(n - 1)];
}
