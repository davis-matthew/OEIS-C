/*
 * A257717  a(n) is the smallest odd-indexed prime not included earlier such
 *          that a(n) + a(n-1) + a(n-2) is a prime, beginning with a(1) = 5
 *          and a(2) = 11.
 * https://oeis.org/A257717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257717: a(1)=5, a(2)=11. 
   For n≥3, a(n) is the smallest odd‑indexed prime not used earlier
   such that a(n)+a(n-1)+a(n-2) is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
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

static seq_t   *seq      = NULL;          /* sequence values */
static idx_t   seq_len   = 0;             /* number of values computed */
static idx_t   seq_cap   = 0;             /* capacity of seq       */

static uint64_t *prime_arr = NULL;       /* array of primes (uint64_t) */
static size_t   prime_cnt   = 0;          /* number of primes built  */
static uint64_t prime_limit = 0;          /* upper bound of primes built */
static uint8_t  *used   = NULL;          /* used[prime_index] flag   */

/* Ensure seq has room for at least 'need' elements */
static void ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if ((idx_t)new_cap < need) new_cap = need;
    seq_t *tmp = seq ? seq_realloc(seq, seq_cap, new_cap) : seq_alloc(new_cap);
    seq = tmp;
    seq_cap = new_cap;
}

/* Extend the prime list to cover at least new_limit, 
   and extend the used array accordingly. */
static void extend_primes(uint64_t new_limit)
{
    uint64_t *tmp;
    size_t   new_cnt;
    build_primes_upto(new_limit, &tmp, &new_cnt);
    free(prime_arr);
    prime_arr = tmp;

    /* Allocate new used array and copy old flags */
    uint8_t *new_used = (uint8_t*)malloc(new_cnt * sizeof(uint8_t));
    for (size_t i = 0; i < prime_cnt; ++i) new_used[i] = used[i];
    free(used);
    used = new_used;

    prime_cnt   = new_cnt;
    prime_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is defined for n ≥ 1 */

    /* First‑time initialisation */
    if (seq == NULL) {
        ensure_seq_cap(16);
        seq[0] = (seq_t)5;          /* a(1) */
        seq[1] = (seq_t)11;         /* a(2) */
        seq_len = 2;

        prime_limit = 1000;
        build_primes_upto(prime_limit, &prime_arr, &prime_cnt);

        used = (uint8_t*)malloc(prime_cnt * sizeof(uint8_t));
        for (size_t i = 0; i < prime_cnt; ++i) used[i] = 0;

        /* Mark primes used in a(1)=5 and a(2)=11 */
        /* 5 is the 3rd prime (index 2), 11 is the 5th prime (index 4) */
        if (prime_cnt > 4) {
            used[2] = 1;  /* prime 5   */
            used[4] = 1;  /* prime 11  */
        }
    }

    /* Compute terms up to requested index n */
    while (seq_len < n) {
        size_t i = 0;  /* iterate over prime indices (0‑based) */
        while (1) {
            /* Grow prime list if needed */
            if (i >= prime_cnt) {
                uint64_t new_lim = prime_limit ? prime_limit << 1 : 1000;
                extend_primes(new_lim);
            }

            /* Consider only odd‑indexed primes (OEIS index = i+1 odd) */
            if ( ((i + 1) & 1) && !used[i] ) {
                seq_t sum = seq[seq_len-1] + seq[seq_len-2] + (seq_t)prime_arr[i];
                if (is_prime_big(sum)) {
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len] = (seq_t)prime_arr[i];
                    seq_len++;
                    used[i] = 1;
                    break;  /* found a(n) */
                }
            }
            i++;
        }
    }

    return seq[n-1];
}
