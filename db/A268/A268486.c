/*
 * A268486  Least k starting a chain of (2n+1) consecutive integers {h(k+i)},
 *          i=0,1,...,2n, where h(k) is the length of the finite set {k, f(k),
 *          f(f(k)), ..., 1} in the Collatz (or 3x + 1) problem, with the
 *          property that h(k) = h(k+2n), h(k+1) = h(k+2n-1), ..., h(k+n-1) =
 *          h(k+n+1).
 * https://oeis.org/A268486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 24, a(2) = 48, a(3) = 230, …
 * a(n) = least k such that the sequence {h(k),h(k+1),…,h(k+2n)} is a palindrome,
 * where h(m) is the length of the finite Collatz trajectory starting from m.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>    /* ptrdiff_t */

static seq_t *h_cache = NULL;         /* cache of Collatz lengths */
static bool   *h_cached = NULL;       /* flags for cached entries */
static size_t h_capacity = 0;

/* ensure the cache can hold index idx (0‑based) */
static void ensure_h_capacity(idx_t idx)
{
    if ((size_t)idx < h_capacity) return;
    size_t new_cap = h_capacity ? h_capacity << 1 : 64;
    if (new_cap <= (size_t)idx) new_cap = (size_t)idx + 1;

    seq_t *tmp = (seq_t *)realloc(h_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    bool *tmpb = (bool *)realloc(h_cached, new_cap * sizeof(bool));
    if (!tmpb) abort();

#ifdef __cplusplus
    /* default‑construct the new seq_t objects */
    for (size_t i = h_capacity; i < new_cap; ++i)
        ::new (&tmp[i]) seq_t();
#endif
    /* new cached flags are false */
    for (size_t i = h_capacity; i < new_cap; ++i)
        tmpb[i] = false;

    h_cache = tmp;
    h_cached = tmpb;
    h_capacity = new_cap;
}

/* Collatz length for n (1‑based). */
static seq_t get_h(idx_t n)
{
    if (n == 0) return 0;
    ensure_h_capacity(n);
    if (h_cached[n]) return h_cache[n];

    /* follow the trajectory until hitting a cached value or 1 */
    idx_t cur = n;
    idx_t *stack = NULL;
    size_t stack_sz = 0, stack_cap = 0;

    while (true) {
        if ((size_t)cur < h_capacity && h_cached[cur])
            break;
        if (cur == 1)
            break;

        if (stack_sz == stack_cap) {
            size_t new_cap = stack_cap ? stack_cap << 1 : 16;
            idx_t *tmp = (idx_t *)realloc(stack, new_cap * sizeof(idx_t));
            if (!tmp) abort();
            stack = tmp;
            stack_cap = new_cap;
        }
        stack[stack_sz++] = cur;

        if (cur & 1U) {
            unsigned __int128 t = (unsigned __int128)cur * 3U + 1U;
            cur = (idx_t)t;
        } else {
            cur >>= 1;
        }
    }

    seq_t len = (cur == 1U) ? (seq_t)1U : h_cache[cur];

    /* unwind stack, filling cache where possible */
    for (ptrdiff_t i = (ptrdiff_t)stack_sz - 1; i >= 0; --i) {
        len = len + (seq_t)1U;
        idx_t val = stack[i];
        if ((size_t)val < h_capacity) {
            h_cache[val] = len;
            h_cached[val] = true;
        }
    }

    if ((size_t)n < h_capacity) {
        h_cache[n] = len;
        h_cached[n] = true;
    }

    free(stack);
    return len;
}

/* answer cache */
static seq_t *answers = NULL;   /* answers[i-1] = a(i) */
static bool   *computed = NULL; /* flags for computed answers */
static size_t ans_capacity = 0;

static void ensure_ans_capacity(idx_t idx)   /* idx is 1‑based */
{
    if ((size_t)idx <= ans_capacity) return;
    size_t new_cap = ans_capacity ? ans_capacity << 1 : 64;
    if (new_cap < (size_t)idx) new_cap = (size_t)idx;

    seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    bool *tmpb = (bool *)realloc(computed, new_cap * sizeof(bool));
    if (!tmpb) abort();

#ifdef __cplusplus
    for (size_t i = ans_capacity; i < new_cap; ++i)
        ::new (&tmp[i]) seq_t();
#endif
    for (size_t i = ans_capacity; i < new_cap; ++i)
        tmpb[i] = false;

    answers = tmp;
    computed = tmpb;
    ans_capacity = new_cap;
}

/* search for the first k with a palindrome of length 2n+1 */
static seq_t find_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t radius = n;
    for (idx_t k = 1; ; ++k) {
        bool ok = true;
        for (idx_t off = 0; off <= radius; ++off) {
            if (get_h(k + off) != get_h(k + 2 * radius - off)) {
                ok = false;
                break;
            }
        }
        if (ok) return k;
    }
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but harmless */
    ensure_ans_capacity(n);
    if (!computed[n - 1]) {
        answers[n - 1] = find_a(n);
        computed[n - 1] = true;
    }
    return answers[n - 1];
}
