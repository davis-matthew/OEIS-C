/*
 * A270667  Nonprime and squarefree Löschian numbers (A003136).
 * https://oeis.org/A270667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 4330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9388 (2026-11-03)
 * Helpers:   is_prime_u64, is_squarefree, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A270667 (OEIS) – Nonprime, square‑free Löschian numbers
 *
 *  A(n) is the n‑th positive integer that can be expressed as
 *      x² + xy + y²  (x , y ≥ 0)
 *  and is both non‑prime and square‑free.  The implementation builds a
 *  cache of the sequence on demand; any index n ≥ 1 may be requested in any
 *  order.  All arithmetic is performed in the build’s seq_t type; a
 *  64‑bit integer representation is also supported.
 *
 *  The routine expands the visited bitmap of Löschian values as needed
 *  and records every eligible number.  The visited bitmap is a byte
 *  array that grows as required; the root double‑sieve loop
 *  recomputes all pairs (x,y) each time it expands, which is very
 *  efficient for the target size (≈10⁴ terms in <2 s).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/isqrt.h"
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

/*--- Static cache for the sequence -----------------------------------*/
static seq_t  *seq     = NULL;          /* array of sequence values      */
static size_t  seq_len = 0;             /* number stored                 */
static size_t  seq_cap = 0;             /* allocated capacity of seq     */

/*--- Visited bitmap of Löschian values (index = value)          ---*/
static bool  *visited      = NULL;     /* visited[v] true if v is Löschian */
static size_t  visited_cap = 0;         /* length of visited array          */
static idx_t   max_val     = 0;         /* largest value already processed  */

/*--- Ensure capacity for the seq array -------------------------------*/
static void
ensure_seq_space(void)
{
    if (seq_len < seq_cap) return;
    seq_cap = seq_cap ? seq_cap << 1 : 8;
    seq = mem_resize(seq_t, seq, seq_cap);
}

/*--- Ensure capacity for the visited bitmap --------------------------*/
static void
ensure_visited_space(size_t new_cap)
{
    if (new_cap <= visited_cap) return;
    visited = (bool *)realloc(visited, new_cap * sizeof(bool));
    memset(visited + visited_cap, 0, (new_cap - visited_cap) * sizeof(bool));
    visited_cap = new_cap;
}

/*--- Expand the sequence until we have at least n terms ----------------*/
static void
expand_to(idx_t n)
{
    while (seq_len < n) {
        /* New upper bound: double each time, start at 10            */
        idx_t new_upper = (max_val) ? (max_val << 1) : 10;
        /* Grow the visited bitmap to cover new_upper */
        ensure_visited_space((size_t)new_upper + 1);

        /* Mark all Löschian numbers ≤ new_upper                */
        idx_t limit = isqrt(new_upper);
        for (idx_t x = 0; x <= limit; ++x) {
            idx_t x2 = x * x;
            for (idx_t y = 0; y <= limit; ++y) {
                idx_t v = x2 + x * y + y * y;
                if (v == 0) continue;
                if (v > new_upper) break;
                visited[v] = true;
            }
        }

        /* Process the new range (max_val, new_upper]            */
        for (idx_t v = max_val + 1; v <= new_upper; ++v) {
            if (!visited[v]) continue;
            seq_t vv = (seq_t)v;
            if (is_prime_big(vv))          /* skip primes           */
                continue;
            if (!is_squarefree(vv))        /* skip non‑square‑free   */
                continue;
            ensure_seq_space();
            seq[seq_len++] = vv;
        }

        max_val = new_upper;
    }
}

/*--- Public function – the OEIS entry                               */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for index 0  */
    if (seq == NULL) {
        seq_cap = 8;
        seq     = mem_new(seq_t, seq_cap);
        seq_len = 0;
    }
    expand_to(n);                        /* guarantees seq_len ≥ n */
    return seq[n - 1];
}
