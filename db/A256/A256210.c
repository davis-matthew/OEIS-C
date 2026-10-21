/*
 * A256210  Lexicographically earliest permutation of positive integers
 *          starting with 2 such that a(a(n)+a(n+1)) is odd for all n.
 * https://oeis.org/A256210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically earliest permutation of positive
 *            integers that starts with 2 and satisfies a(a(n)+a(n+1)) is odd
 *            for all n
 * Range:     u8 n <= 252, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state shared by all calls */
static seq_t *perm   = NULL;   /* 1‑indexed permutation */
static idx_t  perm_cap = 0;
static idx_t  perm_len = 0;

static bool *used   = NULL;    /* used[v] == true if v already chosen */
static idx_t  used_cap = 0;

static bool *req    = NULL;    /* req[pos] == true if a(pos) must be odd */
static idx_t  req_cap = 0;

/* grow perm array to at least 'need' capacity */
static void ensure_perm_cap(idx_t need)
{
    if (need <= perm_cap) return;
    idx_t newcap = perm_cap ? perm_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    perm = (seq_t *)realloc(perm, (newcap + 1) * sizeof(seq_t));
    perm_cap = newcap;
}

/* grow used array to at least 'need' capacity */
static void ensure_used_cap(idx_t need)
{
    if (need <= used_cap) return;
    idx_t newcap = used_cap ? used_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    bool *tmp = (bool *)realloc(used, (newcap + 1) * sizeof(bool));
    for (idx_t i = used_cap + 1; i <= newcap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = newcap;
}

/* grow req array to at least 'need' capacity */
static void ensure_req_cap(idx_t need)
{
    if (need <= req_cap) return;
    idx_t newcap = req_cap ? req_cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    bool *tmp = (bool *)realloc(req, (newcap + 1) * sizeof(bool));
    for (idx_t i = req_cap + 1; i <= newcap; ++i) tmp[i] = false;
    req = tmp;
    req_cap = newcap;
}

/* one‑time initialization of static state */
static void init_sequence(void)
{
    if (perm != NULL) return;          /* already initialized */

    perm_cap = 16;
    perm = (seq_t *)malloc((perm_cap + 1) * sizeof(seq_t));
    perm[1] = (seq_t)2;                 /* first term is 2 */
    perm_len = 1;

    used_cap = 16;
    used = (bool *)malloc((used_cap + 1) * sizeof(bool));
    for (idx_t i = 0; i <= used_cap; ++i) used[i] = false;
    used[2] = true;                     /* 2 is already used */

    req_cap = 16;
    req = (bool *)malloc((req_cap + 1) * sizeof(bool));
    for (idx_t i = 0; i <= req_cap; ++i) req[i] = false;
}

/* public entry point */
seq_t a(idx_t n)
{
    init_sequence();

    /* already computed? */
    if (n <= perm_len) return perm[n];

    /* compute terms up to n */
    for (idx_t cur = perm_len + 1; cur <= n; ++cur)
    {
        ensure_perm_cap(cur);
        ensure_req_cap(cur);

        bool needOdd = req[cur];
        idx_t candidate = 1;

        /* find next unused candidate satisfying the rule */
        for (;; ++candidate)
        {
            ensure_used_cap(candidate);
            if (used[candidate]) continue;          /* already taken */

            if (needOdd && ((candidate & 1) == 0)) continue; /* must be odd */

            seq_t prev = perm[cur - 1];
            idx_t S = (idx_t)prev + candidate;          /* a(cur‑1)+candidate */

            if (S <= perm_len)
            {
                /* position S already known – must be odd */
                if ((perm[S] & 1) == 0) continue;
            }
            else
            {
                /* position S not yet assigned – enforce oddness */
                ensure_req_cap(S);
                req[S] = true;
            }
            break;           /* candidate accepted */
        }

        perm[cur] = (seq_t)candidate;
        used[candidate] = true;
        perm_len = cur;
    }

    return perm[n];
}
