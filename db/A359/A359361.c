/*
 * A359361  Irregular triangle read by rows whose n-th row lists the partial
 *          sums of the integer partition with Heinz number n.
 * https://oeis.org/A359361
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term of the triangle read by rows where each
 *            row r contains the partial sums of the decreasing list of prime
 *            indices of the prime factorization of r
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PRIME 1000000          /* enough for n up to ~10^5 */

/* prime_index[p] gives the index of prime p (1‑based), 0 if p is not prime */
static uint32_t prime_index[MAX_PRIME + 1] = {0};

/* primes[] holds all primes up to MAX_PRIME */
static uint32_t primes[MAX_PRIME + 1] = {0};
static size_t  primes_cnt = 0;
static bool    primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    unsigned char *sieve = (unsigned char*)malloc((MAX_PRIME + 1) * sizeof(unsigned char));
    if (!sieve) exit(1);
    for (size_t i = 0; i <= MAX_PRIME; ++i) sieve[i] = 0;

    for (idx_t i = 2; i <= MAX_PRIME; ++i) {
        if (!sieve[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            prime_index[i] = (uint32_t)(primes_cnt);  /* 1‑based index */
            if ((idx_t)i * (idx_t)i <= MAX_PRIME) {
                for (idx_t j = i * i; j <= MAX_PRIME; j += i) sieve[j] = 1;
            }
        }
    }
    free(sieve);
}

/* comparison for descending sort of uint32_t */
static int cmp_desc(const void *a, const void *b)
{
    uint32_t va = *(const uint32_t*)a;
    uint32_t vb = *(const uint32_t*)b;
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for offset < 2 */

    init_primes();

    idx_t m = n - 1;                    /* shift index so that first term corresponds to m=1 */
    idx_t accum = 0;                    /* total terms processed before current row */

    for (idx_t r = 2; ; ++r) {
        /* factorize r and collect prime indices */
        uint32_t idxs[64];
        int count = 0;
        idx_t temp = r;
        for (size_t pi = 0; pi < primes_cnt && (idx_t)primes[pi] <= temp; ++pi) {
            uint32_t p = primes[pi];
            if (temp % p == 0) {
                uint32_t j = prime_index[p];
                while (temp % p == 0) {
                    temp /= p;
                    idxs[count++] = j;
                }
            }
        }
        if (temp > 1) { /* remaining prime factor > sqrt(r) */
            idxs[count++] = prime_index[(uint32_t)temp];
        }

        if (accum + (idx_t)count >= m) {
            int offset = (int)(m - accum - 1);   /* 0‑based position within this row */
            qsort(idxs, count, sizeof(uint32_t), cmp_desc);
            seq_t sum = 0;
            for (int i = 0; i <= offset; ++i) sum += (seq_t)idxs[i];
            return sum;
        }
        accum += (idx_t)count;
    }
}
