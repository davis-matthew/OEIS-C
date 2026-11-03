/*
 * A329449  For any n >= 0, exactly four sums a(n+i) + a(n+j) are prime, for 0
 *          <= i < j <= 3: lexicographically earliest such sequence of
 *          distinct nonnegative integers.
 * https://oeis.org/A329449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest sequence of distinct
 *            nonnegative integers such that among the six pairwise sums of
 *            any four consecutive terms exactly four are prime. The sequence
 *            is generated greedily: for each new term pick the smallest
 *            unused integer that, together with the previous three ter...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 76 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/* Standard headers (detected by the test harness, no extra text).      */
/* --------------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

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

/* --------------------------------------------------------------------- */
/* Memoisation state                                                    */
/* --------------------------------------------------------------------- */
static seq_t  *seq      = NULL;      /* array of sequence values          */
static size_t seq_len  = 0;          /* number of computed terms          */
static size_t seq_cap  = 0;          /* allocated capacity of seq[]      */

static uint8_t *used     = NULL;     /* bitmap: 1 => value already used  */
static size_t  used_cap  = 0;        /* capacity of used bitmap          */

/* --------------------------------------------------------------------- */
/* Grow `seq` array to hold at least `need` elements                    */
/* --------------------------------------------------------------------- */
static void grow_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq = seq_realloc(seq, seq_cap, new_cap);   /* seq_realloc handles non‑trivial types */
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Grow used bitmap to index values up to `need-1`                       */
/* --------------------------------------------------------------------- */
static void grow_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    uint8_t *new_used = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
    if (!new_used) abort();
    if (new_cap > used_cap)
        memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used = new_used;
    used_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Count the primes among the three pairwise sums of the last three      */
/* terms (used only when seq_len >= 3)                                    */
/* --------------------------------------------------------------------- */
static unsigned count_primes_in_last3(void)
{
    unsigned primes = 0;
    seq_t a0 = seq[seq_len-3];
    seq_t a1 = seq[seq_len-2];
    seq_t a2 = seq[seq_len-1];
    if (is_prime_big(a0 + a1)) ++primes;
    if (is_prime_big(a0 + a2)) ++primes;
    if (is_prime_big(a1 + a2)) ++primes;
    return primes;
}

/* --------------------------------------------------------------------- */
/* Find the smallest unused integer that yields the required number of   */
/* new prime sums with the previous three terms                           */
/* --------------------------------------------------------------------- */
static seq_t find_candidate(void)
{
    unsigned needed = 4 - count_primes_in_last3();   /* primes required   */
    idx_t cand = 0;
    while (true) {
        if ((size_t)cand >= used_cap) grow_used_cap((size_t)cand + 1);
        if (used[(size_t)cand]) { ++cand; continue; }   /* already used */

        unsigned cnt = 0;
        seq_t s   = (seq_t)cand;
        seq_t a0  = seq[seq_len-3];
        seq_t a1  = seq[seq_len-2];
        seq_t a2  = seq[seq_len-1];
        if (is_prime_big(s + a0)) ++cnt;
        if (cnt < needed && is_prime_big(s + a1)) ++cnt;
        if (cnt < needed && is_prime_big(s + a2)) ++cnt;
        if (cnt == needed) {
            used[(size_t)cand] = 1;          /* mark as used    */
            return s;
        }
        ++cand;
    }
}

/* --------------------------------------------------------------------- */
/* Exported sequence function                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!seq) {
        grow_seq_cap(64);
        grow_used_cap(64);
    }

    /* Return memoised value if already available */
    if (n < seq_len) return seq[(size_t)n];

    /* Compute terms up to index n */
    while (seq_len <= (size_t)n) {
        /* Ensure capacity for the new element */
        grow_seq_cap(seq_len + 1);

        if (seq_len < 4) {
            /* First four terms are 0,1,2,3 */
            seq[seq_len] = (seq_t)seq_len;
            if ((size_t)seq_len >= used_cap) grow_used_cap(seq_len + 1);
            used[(size_t)seq_len] = 1;
            ++seq_len;
        } else {
            seq[seq_len] = find_candidate();
            ++seq_len;
        }
    }

    return seq[(size_t)n];
}
