/*
 * A270269  Prime numbers with locations of right angle turns in the Ulam
 *          square spiral that are vertices of isosceles right triangles.
 * https://oeis.org/A270269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For k ≥ 1 let L = 2k+1 and c0 = L², c1 = L² − 2k, c2 = L² − 4k,
 *            c3 = L² − 6k. These are the four corner numbers of the Ulam
 *            square spiral. If at least three of the corners are prime, then
 *            all such primes are vertices of an isosceles right triangle and
 *            belong to the sequence A270269. The primes fo...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *seq   = NULL;   /* cached sequence values          */
static size_t seq_len = 0;    /* number of values in seq         */
static size_t seq_cap = 0;    /* allocated capacity of seq       */
static idx_t   next_k  = 1;   /* next layer to examine           */

static void ensure_cache(idx_t n)
{
    /* Grow the cached sequence until it contains at least n terms. */
    while (seq_len < (size_t)n)
    {
        /* Compute the four corner values for layer k = next_k. */
        seq_t L  = (seq_t)(2 * next_k + 1);
        seq_t L2 = L * L;

        seq_t c[4];
        c[0] = L2;
        c[1] = L2 - (seq_t)(2 * next_k);
        c[2] = L2 - (seq_t)(4 * next_k);
        c[3] = L2 - (seq_t)(6 * next_k);

        int prime_flag[4] = {0};
        int prime_cnt     = 0;

        for (int i = 0; i < 4; ++i)
        {
            if (is_prime_big(c[i]))
            {
                prime_flag[i] = 1;
                ++prime_cnt;
            }
        }

        /* Only add primes when at least three corners are prime. */
        if (prime_cnt >= 3)
        {
            /* Append primes in ascending order: c3, c2, c1, c0. */
            for (int i = 3; i >= 0; --i)
            {
                if (prime_flag[i])
                {
                    if (seq_len == seq_cap)
                        grow_seq(&seq, &seq_cap, seq_len + 3);
                    seq[seq_len++] = c[i];
                }
            }
        }

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is 1‑based; undefined for n=0 */

    if (seq == NULL)
    {
        seq_cap = 1024;
        seq     = seq_alloc(seq_cap);
        seq_len = 0;
    }

    ensure_cache(n);
    return seq[n - 1];
}
