/*
 * A336200  Lexicographically earliest sequence of distinct terms such that
 *          every integer k appears twice, at distance a(k).
 * https://oeis.org/A336200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest sequence of distinct terms such that
 *            every integer k appears twice, at distance a(k)
 * Range:     u8 n <= 452, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compare with the OEIS entry A336200.  The implementation uses
    a greedy construction: for each position n it chooses the
    smallest integer t that has not appeared before and for which a
    second occurrence can be scheduled without conflict.
    The second occurrence is forced at position n + a(t),
    where a(t) is the value already known at index t
    (or, for the special case t == n, a(t) = t).  The same
    indices and values are stored in static arrays that grow as
    needed.  The routine is safe for any integer type defined in
    seq_t and idx_t.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*  seq_t and idx_t are defined by the build (e.g. uint64_t, etc.).  */

static seq_t *seq      = NULL;   /* a[1 .. capacity]            */
static seq_t *forced   = NULL;   /* forced value at a given index (0 = none) */
static idx_t *first   = NULL;   /* first occurrence index of each value (0 = none) */
static idx_t  seq_cap = 0;      /* current allocated capacity for seq / forced */
static idx_t  first_cap = 0;    /* capacity for first array          */
static idx_t  next_needed = 1;  /* next index that still needs to be built */

/* ensure that seq[1..cap] and forced[1..cap] exist and are zeroed */
static void ensure_seq_cap(idx_t cap)
{
    if (cap <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : cap;
    if (new_cap < cap) new_cap = cap;
    seq = (seq_t *)realloc(seq, (size_t)(new_cap + 1) * sizeof(seq_t));
    forced = (seq_t *)realloc(forced, (size_t)(new_cap + 1) * sizeof(seq_t));
    for (idx_t i = seq_cap + 1; i <= new_cap; ++i) {
        seq[i] = 0;
        forced[i] = 0;
    }
    seq_cap = new_cap;
}

/* ensure that first[0..cap] exists and is zeroed */
static void ensure_first_cap(idx_t cap)
{
    if (cap <= first_cap) return;
    idx_t new_cap = first_cap ? first_cap * 2 : cap;
    if (new_cap < cap) new_cap = cap;
    first = (idx_t *)realloc(first, (size_t)(new_cap + 1) * sizeof(idx_t));
    for (idx_t i = first_cap + 1; i <= new_cap; ++i) first[i] = 0;
    first_cap = new_cap;
}

/*  a(n)  --  return the n-th term of the sequence; n ≥ 1.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for 0 */

    /* allocate space up to n if needed; also enlarge the first array   */
    if (n > seq_cap)  ensure_seq_cap(n);
    if (n > first_cap) ensure_first_cap(n);

    /* build any needed terms up to n */
    for (idx_t i = next_needed; i <= n; ++i) {
        if (forced[i] != 0) {                  /* position was forced previously */
            seq[i] = forced[i];
        } else {                               /* choose smallest admissible t */
            idx_t t_candidate = 0;

            for (idx_t t = 1; ; ++t) {
                if (t > first_cap) ensure_first_cap(t);
                if (first[t] != 0) continue;    /* already used once          */
                if (t > i) continue;           /* t cannot be used now       */

                /* distance d = a(t) = seq[t] (known when t < i) or d = t when t == i */
                idx_t d;
                if (t < i) {
                    d = (idx_t)seq[t];
                } else { /* t == i */ 
                    d = t;
                }

                idx_t j = i + d;                /* position of second occurrence */

                if (j > seq_cap) ensure_seq_cap(j);
                if (forced[j] != 0) continue;     /* already forced by someone else */
                if (seq[j] != 0)   continue;     /* already occupied           */

                t_candidate = t;
                break;                             /* smallest admissible t found */
            }

            seq[i] = t_candidate;
            first[t_candidate] = i;                /* remember first occurrence */

            /* schedule the second occurrence now */
            idx_t d;
            if (t_candidate < i) d = (idx_t)seq[t_candidate];
            else                 d = t_candidate;
            idx_t j = i + d;
            if (j > seq_cap) ensure_seq_cap(j);
            forced[j] = t_candidate;
        }
    }

    next_needed = n + 1;
    return seq[n];
}
