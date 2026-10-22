/*
 * A347595  a(0) = 1; for n>0, a(n) is the smallest positive integer that has
 *          not previously occurred such that a(n-1)^2 + n^2 + a(n) is a
 *          square.
 * https://oeis.org/A347595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347595: each term is the smallest unused positive integer
 * that makes a(n-1)^2 + n^2 + a(n) a perfect square.
 *
 * The implementation keeps an array of computed terms and a
 * second array of all used values (which are the same as the terms).
 * It uses 128‑bit arithmetic for intermediate products so that
 * aPrev^2 and (aPrev^2 + i^2) do not overflow for all indices
 * whose terms fit into the native seq_t type.
 *
 * When seq_t is 8 bytes wide (e.g. uint64_t) the code uses the
 * compiler’s unsigned __int128 type for 128‑bit arithmetic.
 * If seq_t is larger the same logic is applied, but the compiler
 * must provide operator overloads for the generic integer class.
 *
 * A binary search computes floor(sqrt(x)) for unsigned 128‑bit values.
 */

#include <stdbool.h>
#include <stdlib.h>

#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
#endif

/* dynamic storage for the sequence and the used set */
static seq_t *seq_arr   = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

static seq_t *used_arr  = NULL;
static size_t used_cap  = 0;
static size_t used_len  = 0;

/*------------------------------------------------------------------*/
/* helpers for dynamic arrays                                      */
static void ensure_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_used(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    used_arr = (seq_t *)realloc(used_arr, new_cap * sizeof(seq_t));
    used_cap = new_cap;
}

static bool used_find(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i) {
        if (used_arr[i] == v) return true;
    }
    return false;
}

static void used_insert(seq_t v)
{
    if (used_find(v)) return;
    ensure_used(used_len + 1);
    used_arr[used_len++] = v;
}

/*------------------------------------------------------------------*/
/* 128‑bit floor square root (works if seq_t is 64‑bit wide)        */
#ifdef __SIZEOF_INT128__
static seq_t sqrt128(u128 x)
{
    u128 low = 0;
    u128 high = ((u128)1 << 64) - 1;   /* sqrt(2^128-1) fits in 64 bits */
    while (low <= high) {
        u128 mid = (low + high) >> 1;
        u128 mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)(low - 1);
}
#endif

/*------------------------------------------------------------------*/
/* initialise the sequence with a(0)=1                            */
static void init_sequence(void)
{
    if (seq_len != 0) return;              /* already initialised */

    ensure_seq(1);
    ensure_used(1);
    seq_arr[0] = (seq_t)1;
    used_insert((seq_t)1);
    seq_len = 1;
}

/*------------------------------------------------------------------*/
/* compute all terms up to and including index n                  */
static void ensure_upto(idx_t n)
{
    init_sequence();
    while (seq_len <= (size_t)n) {
        idx_t i   = (idx_t)seq_len;          /* current index to compute */
        seq_t aPrev = seq_arr[i - 1];

        /* compute base = aPrev^2 + i^2 in 128‑bit if possible */
#ifdef __SIZEOF_INT128__
        u128 base128 = (u128)aPrev * (u128)aPrev + (u128)i * (u128)i;
#else
        /* For arbitrary‑precision seq_t rely on seq_t operators */
        seq_t tmp = aPrev * aPrev + (seq_t)i * (seq_t)i;
        /* We can't compute sqrt of an arbitrary‑precision value with
         * 128‑bit arithmetic, so simply use the provided floor_sqrt
         * which works for the given seq_t type. */
        seq_t base128 = tmp;  /* will be cast back to seq_t below */
#endif

        seq_t k;
#ifdef __SIZEOF_INT128__
        k = sqrt128(base128) + 1;
#else
        k = floor_sqrt(base128) + 1;
#endif

        seq_t aCand = 0;
        /* search for the smallest unused aCand */
        while (true) {
#ifdef __SIZEOF_INT128__
            u128 k128 = (u128)k;
            u128 kSq  = k128 * k128;
            u128 cand128 = kSq - base128;
            aCand = (seq_t)cand128;   /* safe: certifies <= UINT64_MAX for valid terms */
#else
            seq_t kSq = k * k;
            seq_t cand = kSq - (seq_t)base128;
            aCand = cand;
#endif
            if (aCand > 0 && !used_find(aCand)) break;
            ++k;
        }

        ensure_seq(seq_len + 1);
        seq_arr[seq_len++] = aCand;
        used_insert(aCand);
    }
}

/*------------------------------------------------------------------*/
/* required public function                                         */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return seq_arr[(size_t)n];
}
