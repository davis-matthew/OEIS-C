/*
 * A394418  a(1) = 1, a(2) = 2; for n > 2, a(n) is an unused positive number
 *          that shares a factor with a(n-1) while being as close as possible
 *          to a(n-2). If two such numbers exist then the larger is chosen.
 * https://oeis.org/A394418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394418:
   a(1)=1, a(2)=2. For n>2, a(n) is the smallest unused positive number
   that shares a factor with a(n‑1) and that is as close as possible to
   a(n‑2).  If two numbers are equally distant from a(n‑2), choose the
   larger.  The function memoises already‑computed terms and member
   tests via a sorted dynamic array.  All operations use only integer
   arithmetic and the supplied gcd function.  The implementation is
   valid C11 and C++17. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"                /* gcd(seq_t,seq_t) */

/* ---------- storage for the sequence (1‑based) ---------- */
static seq_t   *seq      = NULL;   /* seq[1] .. seq[seq_len] */
static size_t  seq_cap   = 0;      /* allocated length */
static size_t  seq_len   = 0;      /* number of terms stored */

/* ---------- storage for used numbers (sorted) ---------- */
static seq_t   *used_arr  = NULL;  /* sorted list of all used values */
static size_t  used_cap  = 0;      /* allocated length */
static size_t  used_len  = 0;      /* number of used values */

/* ensure seq has capacity for indices up to n (1‑based) */
static void ensure_seq_cap(idx_t n)
{
    if (seq_cap >= (size_t)n + 1) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < (size_t)n + 1) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* ensure used_arr has capacity for at least cap elements */
static void ensure_used_cap(size_t cap)
{
    if (used_cap >= cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 4;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(used_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    used_arr = tmp;
    used_cap = new_cap;
}

/* binary search: return true if val is present in used_arr */
static bool used_contains(seq_t val)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] == val) return true;
        if (used_arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* insert val into used_arr while keeping it sorted.  Caller guarantees
   that val is not already present. */
static void used_insert_sorted(seq_t val)
{
    ensure_used_cap(used_len + 1);
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the insertion point */
    for (size_t i = used_len; i > lo; --i)
        used_arr[i] = used_arr[i - 1];
    used_arr[lo] = val;
    ++used_len;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety guard */
    ensure_seq_cap(n);
    /* generate terms up to n */
    while (seq_len < n) {
        if (seq_len == 0) {                /* a(1) = 1 */
            seq[1] = (seq_t)1;
            used_insert_sorted(seq[1]);
            seq_len = 1;
            continue;
        }
        if (seq_len == 1) {                /* a(2) = 2 */
            seq[2] = (seq_t)2;
            used_insert_sorted(seq[2]);
            seq_len = 2;
            continue;
        }
        /* compute a(seq_len+1) */
        seq_t prevprev = seq[seq_len - 1];
        seq_t prev     = seq[seq_len];
        seq_t candidate = 0;
        bool found = false;
        idx_t d = 0;
        while (!found) {
            seq_t high = prevprev + (seq_t)d;
            bool high_ok = (!used_contains(high) && gcd(high, prev) > (seq_t)1);
            bool low_ok = false;
            seq_t low = 0;
            if (d > 0 && prevprev > (seq_t)d) {
                low = prevprev - (seq_t)d;
                low_ok = (!used_contains(low) && gcd(low, prev) > (seq_t)1);
            }
            if (high_ok || low_ok) {
                if (high_ok && low_ok) {
                    candidate = (high > low) ? high : low;
                } else if (high_ok) {
                    candidate = high;
                } else {
                    candidate = low;
                }
                found = true;
            } else {
                ++d;
            }
        }
        seq[seq_len + 1] = candidate;
        used_insert_sorted(candidate);
        ++seq_len;
    }
    return seq[n];
}
