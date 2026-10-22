/*
 * A331828  Numbers k such that the divisors of k form an addition chain.
 * https://oeis.org/A331828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331828: numbers k such that every divisor of k except 1 can be expressed
 * as the sum of two (possibly equal) other divisors of k.
 * Function: seq_t a(idx_t n)
 *   Returns the nth term (offset 1).  The sequence is built lazily and
 *   cached to answer calls in any order.
 *
 * Implementation details:
 *   • For each integer k >= 1, test whether it satisfies the property.
 *   • Divisors are enumerated by trial division up to sqrt(k).
 *   • For C builds seq_t is a plain integer type; a dynamic array is used for divisors.
 *   • For C++ builds seq_t may be a big‑integer class; in that case std::vector<seq_t>
 *     is used for divisors and for caching sequence values.
 *   • Special case: k == 1 is automatically accepted (vacuously satisfies the
 *     condition).  It is the first term of the sequence.
 *   • Caching is done in seq_arr (C) or seq_vec (C++), growing as needed.
 *
 * All operations respect the limits of seq_t and avoid overflow with 128‑bit
 * intermediates where required.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stddef.h>
#endif
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Shared state */
static idx_t last_k = 0;        /* last checked integer */

/* ===========================  C++ (class seq_t)  ============================== */
#ifdef __cplusplus

static std::vector<seq_t> seq_vec;      /* cached sequence values */

/* Test whether integer k satisfies the divisor‑addition‑chain property. */
static bool is_valid(idx_t k)
{
    if (k == 1) return true;                /* vacuously true */

    std::vector<seq_t> divs;
    divs.reserve(32);                       /* initial guess */

    divs.push_back((seq_t)1);

    /* enumerate divisors */
    for (idx_t d = 2; d * d <= k; ++d) {
        if (k % d == 0) {
            divs.push_back((seq_t)d);
            if (d != k / d)
                divs.push_back((seq_t)(k / d));
        }
    }
    divs.push_back((seq_t)k);

    /* sort divisors (insertion sort) */
    for (size_t i = 1; i < divs.size(); ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* check the addition‑chain property */
    for (size_t i = 1; i < divs.size(); ++i) {   /* skip divisor 1 */
        seq_t target = divs[i];
        size_t left = 0, right = i - 1;
        bool ok = false;
        while (left <= right) {
            seq_t sum = divs[left] + divs[right];
            if (sum == target) { ok = true; break; }
            if (sum < target) ++left;
            else --right;
        }
        if (!ok) return false;
    }
    return true;
}

/* -----------------------------  a()  ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for offset 1 */

    while (seq_vec.size() < n) {
        ++last_k;
        if (is_valid(last_k))
            seq_vec.push_back((seq_t)last_k);
    }
    return seq_vec[n - 1];
}

#endif /* __cplusplus */

/* ===========================  C (plain integer seq_t)  ======================= */
#ifndef __cplusplus

static seq_t *seq_arr = NULL;      /* cached sequence values */
static size_t seq_len = 0;         /* number of cached terms */
static size_t seq_cap = 0;         /* capacity of seq_arr */

/* Test whether integer k satisfies the property. */
static bool is_valid(idx_t k)
{
    if (k == 1) return true;                /* vacuously true */

    /* dynamic array for divisors */
    size_t cnt = 0;
    size_t cap = 8;
    seq_t *divs = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!divs) abort();

    divs[cnt++] = (seq_t)1;

    for (idx_t d = 2; d * d <= k; ++d) {
        if (k % d == 0) {
            if (cnt == cap) { cap *= 2; seq_t *tmp = realloc(divs, cap * sizeof(seq_t)); if (!tmp) abort(); divs = tmp; }
            divs[cnt++] = (seq_t)d;
            if (d != k / d) {
                if (cnt == cap) { cap *= 2; seq_t *tmp = realloc(divs, cap * sizeof(seq_t)); if (!tmp) abort(); divs = tmp; }
                divs[cnt++] = (seq_t)(k / d);
            }
        }
    }
    if (cnt == cap) { cap += 1; seq_t *tmp = realloc(divs, cap * sizeof(seq_t)); if (!tmp) abort(); divs = tmp; }
    divs[cnt++] = (seq_t)k;

    /* insertion sort */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* property check */
    bool ok_all = true;
    for (size_t i = 1; i < cnt && ok_all; ++i) {  /* skip 1 */
        seq_t target = divs[i];
        size_t left = 0, right = i - 1;
        bool ok = false;
        while (left <= right) {
            seq_t sum = divs[left] + divs[right];
            if (sum == target) { ok = true; break; }
            if (sum < target) ++left;
            else --right;
        }
        if (!ok) ok_all = false;
    }

    free(divs);
    return ok_all;
}

/* -----------------------------  a()  ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for offset 1 */

    while (seq_len < n) {
        ++last_k;
        if (is_valid(last_k)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 1;
                seq_t *tmp = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_arr = tmp;
                seq_cap = new_cap;
            }
            seq_arr[seq_len++] = (seq_t)last_k;
        }
    }
    return seq_arr[n - 1];
}

#endif /* !__cplusplus */
