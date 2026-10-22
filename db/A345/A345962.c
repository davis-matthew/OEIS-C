/*
 * A345962  Numbers whose prime indices have alternating sum -2.
 * https://oeis.org/A345962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Compute a(n) by scanning integers in increasing
   order and testing that the alternating sum of the prime indices
   of the integer equals -2. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* sieve limit – large enough for prime factors of candidates up to
   at least 2×10⁶.  The 10 k‑th term is far smaller than this. */
#define SIEVE_LIMIT 2000000

/* static arrays to hold the sieve data */
static unsigned char sieve_composite[SIEVE_LIMIT + 1];
static int prime_index_arr[SIEVE_LIMIT + 1];
static int primes_arr[200000];          /* _more_ than π(2·10⁶) ≈ 148 933 */
static int primes_cnt = 0;
static bool initialized = false;

/* ----- helper: initialize the sieve once ----- */
static void init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* 0 and 1 are not composite */
    sieve_composite[0] = sieve_composite[1] = 1;

    for (int i = 2; i * i <= SIEVE_LIMIT; ++i)
    {
        if (!sieve_composite[i])
        {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_composite[j] = 1;
        }
    }

    /* fill prime list and index array */
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!sieve_composite[i])
        {
            prime_index_arr[i] = primes_cnt + 1; /* 1‑based index */
            primes_arr[primes_cnt++] = i;
        }
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* defensive */
    init_sieve();

    idx_t found = 0;
    /* search candidates starting from 2 (first composite) */
    for (uint64_t cand = 2; ; ++cand)
    {
        uint64_t temp = cand;
        int sign = 1;
        int64_t alt_sum = 0;

        /* trial division with pre‑computed primes */
        for (int pi = 0; pi < primes_cnt; ++pi)
        {
            int p = primes_arr[pi];
            uint64_t pp = (uint64_t)p * (uint64_t)p;
            if (pp > temp) break;
            if ((temp % p) == 0)
            {
                int exp = 0;
                while ((temp % p) == 0)
                {
                    temp /= p;
                    ++exp;
                }
                int idx = prime_index_arr[p];
                for (int k = 0; k < exp; ++k)
                {
                    alt_sum += (int64_t)sign * idx;
                    sign = -sign;
                }
            }
        }

        /* remaining prime factor (if any) */
        if (temp > 1)
        {
            /* temp is prime and <= cand, hence <= SIEVE_LIMIT */
            int idx = prime_index_arr[(int)temp];
            alt_sum += (int64_t)sign * idx;
        }

        if (alt_sum == (int64_t)-2)
        {
            ++found;
            if (found == n)
                return (seq_t)cand;   /* cast to seq_t on return */
        }
    }
    /* unreachable */
    return (seq_t)0;
}
