/*
 * A387119  Numbers whose prime indices all have exactly 2 divisors in common.
 * https://oeis.org/A387119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth number whose prime indices all have exactly 2
 *            divisors in common (OEIS A387119). The function computes the
 *            term by a brute‑force search with precomputed primes and
 *            smallest prime factors
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* --------- initialization --------------------------------------- */
    static int initialized = 0;
    static const int LIMIT = 2000000;        /* sieve limit (≈2·10⁶) */
    static int  *spf          = NULL;       /* smallest prime factor */
    static int  *prime_index  = NULL;       /* index of the prime */
    static unsigned char *idx_prime_flag = NULL; /* prime flag for indices */
    static int   prime_cnt = 0;

    /* dynamic array for cached terms */
    static seq_t  *seq  = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    if (!initialized)
    {
        /* allocate arrays */
        spf          = (int*)calloc((size_t)LIMIT + 1, sizeof(int));
        prime_index  = (int*)calloc((size_t)LIMIT + 1, sizeof(int));
        idx_prime_flag = (unsigned char*)calloc((size_t)LIMIT + 1, sizeof(unsigned char));

        /* linear sieve, store primes in temporary array */
        int *primes = (int*)malloc((size_t)(LIMIT / 10) * sizeof(int));
        size_t primes_cnt = 0;

        for (int i = 2; i <= LIMIT; ++i)
        {
            if (spf[i] == 0)
            {
                spf[i] = i;
                primes[primes_cnt++] = i;
                prime_index[i] = (int)primes_cnt;     /* 1‑based */
            }
            for (size_t j = 0; j < primes_cnt; ++j)
            {
                int p = primes[j];
                long long prod = (long long)i * p;
                if (p > spf[i] || prod > LIMIT) break;
                spf[(int)prod] = p;
            }
        }
        prime_cnt = (int)primes_cnt;

        /* build is‑prime flag for indices 0 … prime_cnt */
        for (int i = 0; i <= prime_cnt; ++i) idx_prime_flag[i] = 1;
        idx_prime_flag[0] = 0;  idx_prime_flag[1] = 0;
        for (int i = 2; i * i <= prime_cnt; ++i)
            if (idx_prime_flag[i])
                for (int j = i * i; j <= prime_cnt; j += i)
                    idx_prime_flag[j] = 0;

        free(primes);
        initialized = 1;
    }

    /* --------- generate terms until we have enough ----------------- */
    if (seq_len < n)
    {
        size_t need = (size_t)n;
        if (seq_cap < need)
        {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            while (new_cap < need) new_cap *= 2;
            seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        uint64_t m_start = seq_len ? (uint64_t)seq[seq_len - 1] + 1 : 3;
        while (seq_len < need)
        {
            int tmp = (int)m_start;
            int g   = 0;
            while (tmp > 1)
            {
                int p = spf[tmp];
                while (tmp % p == 0) tmp /= p;
                int idx = prime_index[p];
                if (g == 0) g = idx;
                else        g = (int)gcd_idx(g, idx);
                if (g == 1) break;
            }
            if (g > 1 && idx_prime_flag[g])
                seq[seq_len++] = (seq_t)m_start;
            ++m_start;
        }
    }
    return seq[n - 1];
}
