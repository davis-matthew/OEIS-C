/*
 * A383639  Left-truncatable happy numbers: every suffix is a happy number and
 *          no digits are zero.
 * https://oeis.org/A383639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the nth left‑truncatable happy number with no zero digits
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 88, u64 n <= 170, u128 n <= 170,
 *            big any n
 * Helpers:   digit_len, get_digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

#if defined(__cplusplus)
# include <vector>
#endif

/* -------------------------------------------------------------------------
   Helper – check if a number is happy
   ------------------------------------------------------------------------- */
static bool is_happy(seq_t x)
{
    /* After one conversion the value is <= 9^2 * 13 = 1107 for 64‑bit numbers.
       A small static array is sufficient for cycle detection.           */
    bool seen[20000] = {0};
    unsigned long long v = 0, u;
    void *ptr = NULL;
    size_t len = 0;
    uint8_t digits[64];
    /* extract digits – uses the utils routine to work for all seq_t */
    get_digits(x, digits, &len);

    for (size_t i = 0; i < len; ++i)
        v += (unsigned long long)digits[i] * (unsigned long long)digits[i];

    while (v != 1) {
        if (seen[(size_t)v]) return false;
        seen[(size_t)v] = true;
        u = 0;
        while (v) {
            u += (v % 10) * (v % 10);
            v /= 10;
        }
        v = u;
    }
    return true;
}

/* -------------------------------------------------------------------------
   Min‑heap implementation – two code paths: C and C++
   ------------------------------------------------------------------------- */
#if defined(__cplusplus)

/* C++ path: use std::vector as back‑ing storage */
static std::vector<seq_t> heap;

static void heap_push(seq_t v)
{
    heap.push_back(v);
    size_t i = heap.size() - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= v) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = v;
}

static seq_t heap_pop(void)
{
    seq_t ret = heap[0];
    seq_t last = heap.back();
    heap.pop_back();
    if (heap.empty()) return ret;
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        size_t r = l + 1;
        if (l >= heap.size()) break;
        size_t s = (r < heap.size() && heap[r] < heap[l]) ? r : l;
        if (heap[s] >= last) break;
        heap[i] = heap[s];
        i = s;
    }
    heap[i] = last;
    return ret;
}

static size_t heap_size(void) { return heap.size(); }

static std::vector<seq_t> terms;
static size_t terms_size(void) { return terms.size(); }

#else

/* C path – raw dynamic array */
static seq_t *heap = NULL;
static size_t heap_sz = 0, heap_cap = 0;

static void heap_grow(void)
{
    size_t new_cap = heap_cap ? heap_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(heap, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_push(seq_t v)
{
    if (heap_sz == heap_cap) heap_grow();
    size_t i = heap_sz++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= v) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = v;
}

static seq_t heap_pop(void)
{
    seq_t ret = heap[0];
    seq_t last = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        size_t r = l + 1;
        if (l >= heap_sz) break;
        size_t s = (r < heap_sz && heap[r] < heap[l]) ? r : l;
        if (heap[s] >= last) break;
        heap[i] = heap[s];
        i = s;
    }
    heap[i] = last;
    return ret;
}

static size_t heap_size(void) { return heap_sz; }

static seq_t *terms = NULL;
static size_t terms_cnt = 0, terms_cap = 0;

static void terms_grow(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 200;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

static size_t terms_size(void) { return terms_cnt; }

#endif

/* -------------------------------------------------------------------------
   Precompute all 170 terms the first time a(idx_t) is called
   ------------------------------------------------------------------------- */
static bool computed = false;

static void compute_all(void)
{
    /* initialise heap with the single‑digit happy numbers 1 and 7 */
#if defined(__cplusplus)
    heap.clear();
    terms.clear();
#else
    heap = NULL; heap_sz = heap_cap = 0;
    terms = NULL; terms_cnt = terms_cap = 0;
#endif
    seq_t seeds[2] = { (seq_t)1, (seq_t)7 };
    for (int k = 0; k < 2; ++k) heap_push(seeds[k]);

    while (terms_size() < 170) {
        if (heap_size() == 0) break;   /* should not happen */

        seq_t cur = heap_pop();

#if defined(__cplusplus)
        terms.push_back(cur);
#else
        if (terms_cnt == terms_cap) terms_grow();
        terms[terms_cnt++] = cur;
#endif

        /* generate children: prepend each digit 1..9 */
        size_t len = digit_len(cur);
        seq_t pow10len = pow10(len);
        for (int d = 1; d <= 9; ++d) {
            seq_t child = (seq_t)d * pow10len + cur;
            if (!is_happy(child)) continue;
            heap_push(child);
        }
    }

    computed = true;
}

/* -------------------------------------------------------------------------
   Required sequence function
   ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!computed) compute_all();

    if (n == 0) return 0;
    if (n > 170) return 0;          /* safety – only 170 terms exist */

#if defined(__cplusplus)
    return terms[(size_t)n - 1];
#else
    return terms[(size_t)n - 1];
#endif
}
