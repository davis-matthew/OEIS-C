/*
 * A367068  a(n) = ((Sum_{i=1..n} A340510(i))-1)/(n+1).
 * https://oeis.org/A367068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((Sum_{i=1..n} A340510(i))-1)/(n+1) Computed by the
 *            recurrence: a(1)=0, a(2)=1, for n>2, let v=a(n-1). If v equals
 *            any A340510(k) for 1≤k<n, then a(n)=v+1; otherwise a(n)=v.
 *            A340510(n) is the Avdispahić–Zejnulahi sequence for k=1,
 *            generated simultaneously with a(n)
 * Range:     u8 n <= 23, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static idx_t max_n = 0;              /* highest n computed so far */
static seq_t *a_arr = NULL;          /* a values, 1‑based */
static seq_t *b_arr = NULL;          /* A340510 values, 1‑based */
static size_t array_cap = 0;         /* capacity of a_arr and b_arr */
static unsigned char *used = NULL;   /* bitmap of used A340510 values */
static size_t used_cap = 0;          /* capacity of used bitmap */
static size_t max_used_value = 0;    /* largest value seen in A340510 */
static seq_t sum_b = 0;              /* running sum of A340510 values */

/* Ensure a_arr and b_arr can hold indices up to n. */
static void ensure_array_cap(idx_t n)
{
    if ((idx_t)array_cap > n) return;
    size_t new_cap = array_cap ? array_cap << 1 : 8;
    while ((size_t)new_cap <= (size_t)n) new_cap <<= 1;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    if (new_cap > array_cap) {
        memset(a_arr + array_cap, 0, (new_cap - array_cap) * sizeof(seq_t));
        memset(b_arr + array_cap, 0, (new_cap - array_cap) * sizeof(seq_t));
    }
    array_cap = new_cap;
}

/* Ensure the used bitmap can index up to idx. */
static void ensure_used_cap(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 8;
    while (new_cap <= idx) new_cap <<= 1;
    used = (unsigned char *)realloc(used, new_cap);
    memset(used + used_cap, 0, new_cap - used_cap);
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for offset <1 */

    ensure_array_cap(n);

    /* Return already computed value if possible. */
    if (n <= max_n) return a_arr[n];

    /* Compute values up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {

        if (k == 1) {                     /* initial values */
            a_arr[1] = 0;
            b_arr[1] = 1;
            sum_b = 1;
            ensure_used_cap(1);
            used[1] = 1;
            if (max_used_value < 1) max_used_value = 1;
            continue;
        }

        if (k == 2) {
            a_arr[2] = 1;
            /* A340510(2): 1 + b2 ≡ 1 (mod 3) → b2 = 3. */
            b_arr[2] = 3;
            sum_b += 3;
            ensure_used_cap(3);
            used[3] = 1;
            if (max_used_value < 3) max_used_value = 3;
            continue;
        }

        /* ----- compute a(k) ----- */
        seq_t v = a_arr[k - 1];
        bool exists = false;
        size_t vi = (size_t)v;
        if (vi <= max_used_value) {
            ensure_used_cap(vi);
            if (used[vi]) exists = true;
        }
        a_arr[k] = exists ? v + 1 : v;

        /* ----- compute b(k) (A340510(k)) ----- */
        seq_t mod = (seq_t)(k + 1);            /* n + 1 */
        seq_t sum_mod = sum_b % mod;
        seq_t r = (mod + 1 - sum_mod) % mod;   /* desired residue modulo mod */
        seq_t cand = r;
        if (cand == 0) cand = mod;            /* smallest positive solution */

        while (1) {
            size_t ci = (size_t)cand;
            ensure_used_cap(ci);
            if (!used[ci]) break;
            cand += mod;
        }

        b_arr[k] = cand;
        sum_b += cand;
        if ((size_t)cand > max_used_value) max_used_value = (size_t)cand;
        used[(size_t)cand] = 1;
    }

    max_n = n;
    return a_arr[n];
}
