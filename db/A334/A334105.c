/*
 * A334105  Numbers m for which A329697(m) = 5.
 * https://oeis.org/A334105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 4325, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   factorize, hm_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334105: Numbers m for which A329697(m) = 5.
   The function scans integers in increasing order and keeps those
   that can be reduced to a power of two in exactly five steps by the
   operation m := m / p * (p – 1), where p is an odd prime divisor of m.
   The result is returned as the nth such integer (offset 1). */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/factorize.h"
#include "../utils/hm_hash.h"

/* --------------------------------------------------------------------- */
/*  Hash table for memoising minimal step counts for 64‑bit numbers      */
/* --------------------------------------------------------------------- */

#define HM_INITIAL_CAP (1ULL << 16)   /* 65536 entries, power of two */
#define HM_LOAD_FACTOR 0.75f

struct hm_entry {
    uint64_t key;  /* 0 is reserved for an empty slot */
    int      value;
};

static struct hm_entry *hm = NULL;  /* hash table array         */
static size_t            hm_cap = 0;/* current capacity         */
static size_t            hm_used = 0;/* number of occupied cells */

static void hm_init(void)
{
    if (hm)
        return;
    hm_cap = HM_INITIAL_CAP;
    hm = (struct hm_entry *)calloc(hm_cap, sizeof(struct hm_entry));
    if (!hm)
        abort(); /* memory exhaustion – halt */
    hm_used = 0;
}

/* Forward prototypes so that hm_rehash can call them. */
static void hm_put(uint64_t key, int value);
static int  hm_get(uint64_t key, int *out);

static void hm_rehash(void)
{
    size_t old_cap = hm_cap;
    struct hm_entry *old = hm;
    hm_cap <<= 1;                 /* double capacity */
    hm = (struct hm_entry *)calloc(hm_cap, sizeof(struct hm_entry));
    if (!hm)
        abort();
    hm_used = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old[i].key)
            hm_put(old[i].key, old[i].value);
    }
    free(old);
}

static void hm_put(uint64_t key, int value)
{
    hm_init();
    if ((float)(hm_used + 1) / (float)hm_cap > HM_LOAD_FACTOR)
        hm_rehash();

    uint64_t h = hm_hash(key);
    size_t idx = h & (hm_cap - 1);

    while (hm[idx].key && hm[idx].key != key) {
        idx = (idx + 1) & (hm_cap - 1);
    }

    if (!hm[idx].key)
        ++hm_used;
    hm[idx].key   = key;
    hm[idx].value = value;
}

static int hm_get(uint64_t key, int *out)
{
    hm_init();
    uint64_t h = hm_hash(key);
    size_t idx = h & (hm_cap - 1);

    while (hm[idx].key) {
        if (hm[idx].key == key) {
            *out = hm[idx].value;
            return 1;
        }
        idx = (idx + 1) & (hm_cap - 1);
    }
    return 0;
}

/* --------------------------------------------------------------------- */
/*  Minimal step count for a single number                               */
/* --------------------------------------------------------------------- */

static int steps_for_m(uint64_t m)
{
    if (m <= 1)
        return 0;                  /* 1 is already a power of two */

    /* strip all factors of two */
    while ((m & 1ULL) == 0)
        m >>= 1;
    if (m <= 1)
        return 0;

    int cached;
    if (hm_get(m, &cached))
        return cached;

    /* factorisation into distinct primes */
    uint64_t primes[64];
    int     exps[64];
    size_t  pcnt = factorize(m, primes, exps);   /* returns number of distinct primes */

    int best = INT32_MAX; /* steps count is non‑negative and small */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t next = (m / p) * (p - 1);
        int r = 1 + steps_for_m(next);
        if (r < best)
            best = r;
        if (best == 2)          /* cannot be better than 2 */
            break;
    }

    hm_put(m, best);
    return best;
}

/* --------------------------------------------------------------------- */
/*  Sequence cache: list of terms with exactly five steps                */
/* --------------------------------------------------------------------- */

static seq_t *seq_buf = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint64_t last_checked = 1; /* last m that was examined */

static void extend_seq(size_t n)
{
    while (seq_len < n) {
        ++last_checked;
        if (steps_for_m(last_checked) == 5) {
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0 ? 16 : seq_cap << 1);
                seq_buf = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
                if (!seq_buf)
                    abort();
            }
            seq_buf[seq_len++] = (seq_t)last_checked;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public entry point                                                   */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* safety for offset 1 */
    extend_seq((size_t)n);
    return seq_buf[n - 1];
}
