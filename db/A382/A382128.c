/*
 * A382128  Fractalization of the Recamán sequence.
 * https://oeis.org/A382128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán fractalization (OEIS A382128)
   a(2n) = a(n);
   a(2n-1) = Recamán(n), n ≥ 1
   Recamán sequence (1‑based): R(1)=0, for i≥2:
   R(i) = R(i-1)-k if R(i-1)≥k and R(i-1)-k not seen,
          otherwise R(i)=R(i-1)+k, where k=i-1.
   The function memoizes both Recamán values and the sequence itself
   to support arbitrary call order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Recamán memoization ---------- */
static seq_t *recam_vals = NULL;   /* 1‑based Recamán values in recam_vals[0] */
static size_t recam_len  = 0;      /* number of computed Recamán terms      */
static size_t recam_cap  = 0;      /* capacity of recam_vals                */
static unsigned char *seen  = NULL;/* bitset of seen Recamán values          */
static size_t seen_cap      = 0;   /* size of seen array                    */

/* enlarge seen array to cover index up to val */
static void
ensure_seen_cap(seq_t val)
{
    size_t needed = (size_t)val + 1;
    if (seen_cap >= needed)
        return;
    size_t new_cap = seen_cap ? seen_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
    if (!tmp)
        return;            /* allocation failure – should not happen */
    for (size_t i = seen_cap; i < new_cap; ++i)
        tmp[i] = 0;
    seen = tmp;
    seen_cap = new_cap;
}

/* ensure Recamán values up to index 'need' are computed */
static void
ensure_recam_idx(idx_t need)
{
    if ((size_t)need <= recam_len)
        return;

    /* grow recam_vals array if necessary */
    size_t new_cap = recam_cap ? recam_cap : 16;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    if (new_cap > recam_cap) {
        recam_vals = (seq_t *)realloc(recam_vals, new_cap * sizeof(seq_t));
        recam_cap  = new_cap;
    }

    /* initialize first Recamán term */
    if (recam_len == 0) {
        recam_vals[recam_len++] = (seq_t)0;   /* R(1) = 0 */
        ensure_seen_cap(0);
        seen[0] = 1;
    }

    /* generate subsequent Recamán terms */
    while (recam_len < (size_t)need) {
        seq_t prev = recam_vals[recam_len - 1];
        seq_t k    = (seq_t)recam_len;   /* i-1 for new index i = recam_len+1 */

        seq_t next;
        if (prev >= k) {
            seq_t cand = prev - k;
            if (!seen[(size_t)cand])
                next = cand;
            else
                next = prev + k;
        } else
            next = prev + k;

        recam_vals[recam_len++] = next;
        ensure_seen_cap(next);
        seen[(size_t)next] = 1;
    }
}

/* return Recamán(k) 1‑based */
static seq_t
recam_val(idx_t k)
{
    if (k < 1)
        return (seq_t)0;
    ensure_recam_idx(k);
    return recam_vals[(size_t)(k - 1)];
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* offset starts at 1 */
    if ((n & 1) == 0)                /* even index: a(n) = a(n/2) */
        return a(n / 2);
    else                              /* odd index:   a(n) = Recamán((n+1)/2) */
    {
        idx_t k = (n + 1) / 2;
        return recam_val(k);
    }
}
