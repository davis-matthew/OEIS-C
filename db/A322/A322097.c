/*
 * A322097  a(1)=1, a(2)=1; for n > 2, a(n) is the largest proper divisor of
 *          the concatenation of terms a(1) through a(n-1).
 * https://oeis.org/A322097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=1; for n>2, a(n) = c(n-1)/spf(c(n-1)), where c(k)
 *            is the decimal concatenation of a(1)…a(k) and spf(m) is the
 *            smallest prime factor of m (m itself if m is prime). digit_len
 *            and pow10 give decimal digit count and 10^e respectively
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  Utilities from the build system                           */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/*  Sequence cache                                               */
static seq_t *cache_a   = NULL;   /* 1-based: cache_a[1] = a(1)          */
static size_t cache_cap = 0;
static idx_t   cache_max = 0;    /* largest computed index */
static seq_t   cache_c   = 0;    /* concatenation c(k)                */

/* ------------------------------------------------------------ */
/*  Helper to ensure that cache_a has space for at least n+1   */
static void ensure_cache_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = mem_resize(seq_t, cache_a, new_cap);
    cache_a   = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Smallest prime factor (full factorisation for arbitrary    */
/*  precision – Pollard Rho + recursion).                        */
static seq_t smallest_prime_factor(seq_t n)
{
    /* Handle trivial cases */
    if (n <= 1)
        return n;

    /* Even number */
    if ((n & 1) == 0)
        return (seq_t)2;

    /* Trial division by small primes up to 2000 for a quick win */
    static const unsigned int small_primes[] = {
         3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
         37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
         79, 83, 89, 97,101,103,107,109,113,127,
        131,137,139,149,151,157,163,167,173,179,
        181,191,193,197,199,211,223,227,229,233,
        239,241,251,257,263,269,271,277,281,283,
        293,307,311,313,317,331,337,347,349,353,
        359,367,373,379,383,389,397,401,409,419,
        421,431,433,439,443,449,457,461,463,467,
        479,487,491,499,503,509,521,523,541,547,
        557,563,569,571,577,587,593,599,601,607,
        613,617,619,631,641,643,647,653,659,661,
        673,677,683,691,701,709,719,727,733,739,
        743,751,757,761,769,773,787,797,809,811,
        821,823,827,829,839,853,857,859,863,877,
        881,883,887,907,911,919,929,937,941,947,
        953,967,971,977,983,991,997
    };
    const size_t nb_small = sizeof(small_primes) / sizeof(small_primes[0]);
    for (size_t i = 0; i < nb_small; ++i) {
        seq_t p = (seq_t)small_primes[i];
        if (p * p > n)
            break;
        if ((n % p) == 0)
            return p;
    }

    /* If the number is prime, return it */
    if (is_prime_big(n))
        return n;

    /* Pollard's Rho to find a non‑trivial factor */
    static uint64_t poll_seed = 123456789ULL;
    seq_t d = 1;
    while (d == 1) {
        seq_t x = (seq_t)(2);
        seq_t y = (seq_t)(2);
        seq_t c = (seq_t)(++poll_seed % (n - (seq_t)2) + (seq_t)2);
        do {
            /* f(t) = t*t + c mod n */
            x = (x * x + c) % n;
            y = (y * y + c) % n;
            y = (y * y + c) % n;
            d = gcd((x > y) ? (x - y) : (y - x), n);
        } while (d == 1);
        if (d == n) {
            /* failure – retry with a new constant */
            poll_seed++;
            continue;
        }
    }

    /* Recursively find the minimal factor between d and n/d */
    seq_t d1 = smallest_prime_factor(d);
    seq_t d2 = smallest_prime_factor(n / d);
    return (d1 < d2) ? d1 : d2;
}

/* ------------------------------------------------------------ */
/*  Ensure sequence is computed up to n (inclusive)             */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    ensure_cache_capacity(n);

    /* First term */
    if (cache_max == 0) {
        cache_a[1] = (seq_t)1;
        cache_c    = (seq_t)1;   /* c(1) = a(1) */
        cache_max  = 1;
    }

    /* Second term */
    if (cache_max == 1 && n >= 2) {
        cache_a[2] = (seq_t)1;                   /* a(2) = 1 */
        cache_c = cache_c * pow10(1) + cache_a[2];/* c(2) = 11    */
        cache_max = 2;
    }

    /* Remaining terms */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t spf = smallest_prime_factor(cache_c);
        seq_t a_k = cache_c / spf;
        cache_a[k] = a_k;

        /* Update concatenation: c(k) = c(k-1)*10^d + a_k */
        int d = num_digits(a_k);
        cache_c = cache_c * pow10((idx_t)d) + a_k;
    }

    cache_max = n;
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                    */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined, safety value */

    ensure_cache(n);
    return cache_a[n];
}
