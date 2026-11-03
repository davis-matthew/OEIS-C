/*
 * A330125  Positive integers whose digit-power sum is a prime.
 * https://oeis.org/A330125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1626, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9695 (2026-11-03)
 * Helpers:   get_digits, ipow, is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A330125. 
   A(n) is the nth positive integer M whose digit‐power sum is prime, 
   i.e. if M has k decimal digits d_1 … d_k then 
   Σ_{i=1..k} d_i^k is a prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/get_digits.h"
#include "../utils/ipow.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *arr     = NULL;   /* array of sequence values */
    static size_t arr_len = 0;      /* number of values currently stored */
    static size_t arr_cap = 0;      /* allocated capacity of arr       */
    static idx_t  next_candidate = 1; /* next integer to test           */
    static idx_t  found         = 0;  /* how many values have been found */

    /* ensure array is large enough for the desired index */
    if (n > arr_len)
    {
        /* make the array large enough */
        size_t need = n;
        if (arr_cap < need)
        {
            size_t newcap = (arr_cap == 0) ? 16 : arr_cap;
            while (newcap < need) newcap <<= 1;
            seq_t *tmp = mem_resize(seq_t, arr, newcap);
            if (!tmp) return 0; /* failure – should not happen */
            arr     = tmp;
            arr_cap = newcap;
        }

        /* generate values until we have at least n of them */
        while (found < n)
        {
            seq_t m = (seq_t)next_candidate; /* candidate integer */

            /* number of decimal digits of m */
            int N = num_digits(m);

            /* compute sum of digit^N */
            seq_t sum = 0;
            uint8_t digits[32];
            size_t digits_len;
            get_digits(m, digits, &digits_len);

            for (size_t i = 0; i < digits_len; ++i)
            {
                seq_t d   = (seq_t)digits[i];
                seq_t pow = ipow(d, (seq_t)N);
                sum += pow;
            }

            /* test primality of the sum */
            if (isPrimeGeneric(sum))
            {
                arr[found++] = m;
            }
            ++next_candidate;
        }
    }

    return arr[n - 1];
}
