/*
 * A319736  The lexicographically earliest increasing sequence such that n
 *          divides the sum of the first a(n) terms.
 * https://oeis.org/A319736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A319736 – lexicographically earliest increasing sequence
 *            satisfying : for all n ≥ 1, n divides Σ_{k=1}^{a(n)} a(k)
 * Range:     u8 n <= 17, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static storage for the sequence */
static seq_t *seq          = NULL;   /* 1‑based array of terms               */
static size_t seq_cap      = 0;       /* allocated capacity for `seq`        */
static idx_t seq_len       = 0;       /* number of terms already computed    */

/* constraints: for a value v, constraint[v] is the index n such that
   a(n)=v (0 if none).  When a(n)=v is discovered we set
   constraint[v]=n.  This will enforce that n | S(v) when we reach
   index v. */
static idx_t *constraint   = NULL;   /* 1‑based array of constraints        */
static size_t cons_cap     = 0;       /* allocated capacity for `constraint` */

static seq_t total_sum = 0;          /* S(seq_len) = Σ_{k=1}^{seq_len} a(k) */

/* resize seq array to hold at least one‑based index `idx` */
static void ensure_seq_size(idx_t idx)
{
    if (seq_cap <= (size_t)idx) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* resize constraint array to hold at least one‑based index `idx` */
static void ensure_con_size(idx_t idx)
{
    if (cons_cap <= (size_t)idx) {
        size_t new_cap = cons_cap ? cons_cap << 1 : 16;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        constraint = (idx_t *)realloc(constraint, new_cap * sizeof(idx_t));
        /* initialisation of newly allocated slots to zero */
        for (size_t i = cons_cap; i < new_cap; ++i) constraint[i] = 0;
        cons_cap = new_cap;
    }
}

/* initialise static arrays on first call */
static void initialise(void)
{
    if (seq != NULL) return;           /* already initialised */
    /* sequence: a(1)=1 */
    seq_cap = 16;
    seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_len = 1;
    seq[1] = 1;
    total_sum = 1;

    cons_cap = 16;
    constraint = (idx_t *)calloc(cons_cap, sizeof(idx_t));
    constraint[1] = 1;                 /* enforce 1 | S(1) */
}

/* construct terms up to index `n` (inclusive) */
static void build_to(idx_t n)
{
    initialise();

    while (seq_len < n) {
        idx_t i = seq_len + 1;            /* next index to compute */

        ensure_con_size(i);               /* ensure we can read constraint[i] */

        seq_t prev   = seq[seq_len];      /* a(i‑1) */
        seq_t start  = prev + 1;          /* smallest candidate > a(i‑1) */

        idx_t mod = constraint[i];        /* divisor coming from a(n)=i (if any) */
        seq_t candidate = start;

        if (mod != 0) {
            /* we need S(i) % mod == 0, i.e. (total_sum + candidate) % mod == 0
               => candidate ≡ (-total_sum) (mod mod)      */
            seq_t desired = ((seq_t)mod - (total_sum % (seq_t)mod)) % (seq_t)mod;
            seq_t offset  = (desired + (seq_t)mod - (start % (seq_t)mod)) % (seq_t)mod;
            candidate = start + offset;
        }

        /* Self‑divisibility: if we choose a(i)=i, we must also have i | S(i) */
        if (candidate == i) {
            if (((total_sum + candidate) % (seq_t)i) != 0) {
                /* adjust candidate to the next value that respects the
                   constraint above (either by adding `mod` or by +1) */
                if (mod != 0)
                    candidate += (seq_t)mod;
                else
                    candidate += 1;
            }
        }

        /* store the new term */
        ensure_seq_size(i);
        seq[i] = candidate;
        seq_len = i;
        total_sum += candidate;

        /* record the future constraint for value `candidate` */
        idx_t future = (idx_t)candidate;
        ensure_con_size(future);
        constraint[future] = i;
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    build_to(n);
    return seq[n];
}
