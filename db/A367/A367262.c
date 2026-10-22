/*
 * A367262  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that the values a(0) XOR ... XOR a(k) (for some k >=
 *          0) are all distinct (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A367262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A367262
 *  Lexicographically earliest sequence of distinct nonnegative integers
 *  such that all prefix XORs a(0)⊕…⊕a(k) are distinct.
 *
 *  The sequence is a permutation of the nonnegative integers.  The
 *  algorithm stores all previously used a-values and prefix XORs.
 *  For every n it chooses the smallest unused integer that keeps the
 *  new prefix XOR unique.
 *
 *  The implementation uses dynamic arrays that grow by doubling.  For
 *  the build with arbitrary‑precision seq_t the array of a-values is
 *  reallocated with a cast; the other arrays are byte flags.  The
 *  function works for any order of calls and satisfies the time
 *  requirement for n up to roughly 10000. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static state shared between calls */
static seq_t *seq          = NULL;   /* sequence values */
static idx_t seq_len        = 0;      /* number of computed terms   */
static size_t seq_cap       = 0;      /* capacity of seq array      */
static seq_t prefix_xor     = (seq_t)0;   /* XOR of a(0)…a(k-1)    */

static unsigned char *used_a  = NULL;   /* used a-values flag array  */
static size_t used_a_cap      = 0;      /* capacity of used_a       */

static unsigned char *used_s  = NULL;   /* used prefix XOR flags     */
static size_t used_s_cap      = 0;      /* capacity of used_s       */

/* ------------------------------------------------------------------ */
/* Ensure seq array has capacity at least 'needed'.  Uses realloc
 * with an explicit cast so the code compiles in C++ when seq_t is a
 * non‑POD type.  The new entries are implicitly zeroed because
 * realloc keeps existing data; new positions are written before use. */
static void ensure_seq_cap(size_t needed)
{
    if (needed < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap <= needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure used_a flag array accommodates indices up to 'val'.          */
static void ensure_used_a(uint64_t val)
{
    if (val < used_a_cap) return;
    size_t new_cap = used_a_cap ? used_a_cap * 2 : 128;
    while (new_cap <= val) new_cap <<= 1;
    used_a = (unsigned char *)realloc(used_a, new_cap);
    memset(used_a + used_a_cap, 0, new_cap - used_a_cap);
    used_a_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure used_s flag array accommodates indices up to 'val'.          */
static void ensure_used_s(uint64_t val)
{
    if (val < used_s_cap) return;
    size_t new_cap = used_s_cap ? used_s_cap * 2 : 128;
    while (new_cap <= val) new_cap <<= 1;
    used_s = (unsigned char *)realloc(used_s, new_cap);
    memset(used_s + used_s_cap, 0, new_cap - used_s_cap);
    used_s_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute the nth term of A367262.  If terms up to n have not yet
 * been generated, the function generates them in order.               */
seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (seq_len == 0)
    {
        ensure_seq_cap(1);
        seq[0]  = (seq_t)0;
        seq_len = 1;
        prefix_xor = (seq_t)0;
        ensure_used_a(0);
        used_a[0] = 1;
        ensure_used_s(0);
        used_s[0] = 1;
    }

    /* Generate terms up to n (if required) */
    while (seq_len <= n)
    {
        uint64_t cand = 0;
        for (;; ++cand)
        {
            /* skip values already used as a(n)               */
            ensure_used_a(cand);
            if (used_a[cand]) continue;

            /* compute candidate prefix XOR                   */
            seq_t new_xor = prefix_xor ^ (seq_t)cand;
            uint64_t idx = (uint64_t)new_xor;     /* index into used_s */

            /* ensure used_s array large enough             */
            ensure_used_s(idx);
            if (used_s[idx]) continue;            /* XOR already seen   */

            /* Accept candidate                          */
            seq_len++;
            ensure_seq_cap(seq_len - 1);
            seq[seq_len - 1] = (seq_t)cand;
            used_a[cand] = 1;
            used_s[idx]  = 1;
            prefix_xor   = new_xor;
            break;
        }
    }

    return seq[n];
}
