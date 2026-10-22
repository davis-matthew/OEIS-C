/*
 * A360968  Permutation of the positive integers derived through a process of
 *          self-reference and self-editing. a(1) = 1. Other terms generated
 *          as described in Comments.
 * https://oeis.org/A360968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360968: permutation constructed by a self‑referential
   editing process.
   a(1) = 1.
   For each unedited a(n) in increasing order:
        let p be the number of already processed unedited indices
        (starting at 1);
        q   = n + a(p);
        if a(q) has already been edited, set a(n) = a(q);
        else set a(n) = q;
        then set a(q) = n and mark both a(n) and a(q) as edited.
   The function memoises the construction and returns a(n) for any n ≥ 1. */

#ifndef __cplusplus
/* ======================  C implementation  ====================== */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static mutable state */
static seq_t *perm   = NULL; /* a(i) values, 1‑based; index 0 unused */
static bool  *edited = NULL; /* editing flag for each index */
static idx_t  size   = 0;    /* current allocated size (index 0 … size-1) */
static idx_t  maxComp   = 0; /* largest i that has been processed as n */
static idx_t  ptr_p     = 1; /* current p in the algorithm */
static idx_t  nextUned  = 2; /* next candidate n that is unedited */

/* grow arrays so that indices 0 … req are valid */
static void ensure_capacity(idx_t req)
{
    if (req < size)
        return;
    idx_t newSize = size ? size * 2 : 2;
    if (newSize < req + 1)
        newSize = req + 1;

    seq_t *newPerm = (seq_t *)realloc(perm, newSize * sizeof(seq_t));
    bool  *newEd   = (bool  *)realloc(edited, newSize * sizeof(bool));
    if (!newPerm || !newEd)
        abort(); /* allocation failed */

    /* initialise new entries */
    for (idx_t i = size; i < newSize; ++i) {
        newPerm[i] = (seq_t)i;   /* nominal value */
        newEd[i]   = false;      /* unedited */
    }

    perm   = newPerm;
    edited = newEd;
    size   = newSize;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; return 0 for safety */
        return (seq_t)0;

    /* first‑use initialisation */
    if (!perm) {
        ensure_capacity(1);          /* indices 0 and 1 */
        perm[0]   = (seq_t)0;
        perm[1]   = (seq_t)1;
        edited[0] = false;
        edited[1] = true;            /* a(1) is already fixed */
        maxComp   = 1;
        ptr_p     = 1;
        nextUned  = 2;
    }

    /* grow until we have processed at least up to n */
    while (maxComp < n) {

        /* make sure nextUned exists and is still unedited */
        ensure_capacity(nextUned);
        while (edited[nextUned]) {
            ++nextUned;
            ensure_capacity(nextUned);
        }

        idx_t i = nextUned;          /* current n to process */

        /* ensure a(p) is available */
        ensure_capacity(ptr_p);
        seq_t val_p = perm[ptr_p];   /* a(p) */

        idx_t q = i + (idx_t)val_p;  /* compute q */
        ensure_capacity(q);

        if (edited[q]) {            /* a(q) already edited */
            perm[i] = perm[q];
        } else {
            perm[i] = (seq_t)q;
        }
        perm[q] = (seq_t)i;          /* set a(q) = n */

        edited[i] = true;
        edited[q] = true;

        maxComp  = i;
        ++ptr_p;
        ++nextUned;
    }

    return perm[n];
}
#else
/* ======================  C++ implementation  ====================== */
#include <vector>
#include <cstdint>
#include <cstddef>

/* static mutable state */
static std::vector<seq_t> perm;   /* a(i) values, 1‑based; index 0 unused */
static std::vector<bool>  edited; /* editing flag for each index */
static idx_t  ptr_p     = 1; /* current p in the algorithm */
static idx_t  nextUned  = 2; /* next candidate n that is unedited */

/* ensure capacity up to req (inclusive) */
extern "C" void ensure_capacity_cpp(idx_t req)
{
    if (perm.size() > req)
        return;
    perm.resize(req + 1);
    edited.resize(req + 1, false);
    for (idx_t i = perm.size(); i-- > 0; ) {
        if (perm[i] == (seq_t)0)   // newly added
            perm[i] = (seq_t)i;    // nominal value
    }
    edited[0] = false;          /* index 0 unused */
    edited[1] = true;           /* a(1) fixed initially */
}

/* separate function to allow forward declaration used by seq function */
static seq_t *perm_ptr = nullptr;

/* c++ version of the array growth, called from the libc++ build */
static void ensure_capacity(idx_t req)
{
    ensure_capacity_cpp(req);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* initialise on first use */
    if (perm.empty()) {
        ensure_capacity_cpp(1); /* indices 0 and 1 */
        ptr_p    = 1;
        nextUned = 2;
    }

    /* grow until we have processed up to n */
    idx_t maxComp = ptr_p; /* number of processed n so far */
    if (maxComp < n) {
        /* we will loop, updating maxComp to the last processed n */
        while (maxComp < n) {
            /* make sure nextUned exists and is unedited */
            ensure_capacity_cpp(nextUned);
            while (edited[nextUned]) {
                ++nextUned;
                ensure_capacity_cpp(nextUned);
            }

            idx_t i = nextUned;        /* current n */

            seq_t val_p = perm[ptr_p]; /* a(p) */
            idx_t q = i + (idx_t)val_p;/* compute q */
            ensure_capacity_cpp(q);

            if (edited[q])            /* a(q) already edited */
                perm[i] = perm[q];
            else
                perm[i] = (seq_t)q;
            perm[q] = (seq_t)i;      /* set a(q) = n */

            edited[i] = true;
            edited[q] = true;

            maxComp = i;
            ++ptr_p;
            ++nextUned;
        }
    }

    return perm[n];
}
#endif
