/* mem: arrays and raw-memory routines that are valid at every width,
 * including big, where seq_t is a class that malloc, memset and memcpy
 * cannot create, clear or copy.
 *   mem_new(T, n)        n elements of type T, zero; NULL if out of memory
 *   mem_resize(T, p, n)  like realloc, counted in elements; p may be NULL
 *   mem_free(p)          frees what mem_new or mem_resize returned (or NULL)
 *   mem_set, mem_copy, mem_move, mem_cmp, mem_qsort
 *                        memset, memcpy, memmove, memcmp and qsort, with the
 *                        same arguments (sizes in bytes)
 *   mem_strdup(s)        a malloc'd copy of the string s (strdup is POSIX, not C11)
 * In C, and in C++ for types that are plain memory, these are the C library
 * routines. Other types live in a block that records its element count, so
 * that resizing and freeing can move and destroy them.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MEM_H
#define OEISC_UTILS_MEM_H
#include <stdlib.h>
#include <string.h>

static inline char *mem_strdup(const char *s)
{
    size_t n = strlen(s) + 1;
    char *d = (char *)malloc(n);
    return d ? (char *)memcpy(d, s, n) : d;
}

#ifndef __cplusplus

/* __typeof__(T) * is also a pointer when T is an array type, seq_t[7] */
#define mem_new(T, n) ((__typeof__(T) *)calloc((size_t)(n), sizeof(T)))
#define mem_resize(T, p, n) ((__typeof__(T) *)realloc((p), (size_t)(n) * sizeof(T)))
#define mem_free(p) free(p)
#define mem_set(p, c, n) memset((p), (c), (n))
#define mem_copy(d, s, n) memcpy((d), (s), (n))
#define mem_move(d, s, n) memmove((d), (s), (n))
#define mem_cmp(a, b, n) memcmp((a), (b), (n))
#define mem_qsort(b, n, s, f) qsort((b), (n), (s), (f))

#else

#include <algorithm>
#include <new>
#include <type_traits>
#include <utility>
#include <vector>

namespace oeisc_mem {

const size_t HEADER = 16;    /* keeps the elements 16-byte aligned */

template <class T> struct plain {
    typedef typename std::remove_cv<T>::type U;
    static const bool value = std::is_void<U>::value || std::is_trivially_copyable<U>::value;
};

template <class T> size_t &count(T *p)
{
    return *((size_t *)(((char *)(p)) - HEADER));
}

/* Arrays of arrays (seq_t (*)[K]) are handled as their innermost elements. */
template <class T> using elem = typename std::remove_all_extents<T>::type;
template <class T> constexpr size_t per() { return sizeof(T) / sizeof(elem<T>); }

template <class T> T *make(size_t n)
{
    if constexpr (plain<T>::value) {
        return static_cast<T *>(calloc(n, sizeof(T)));
    } else {
        typedef elem<T> E;
        size_t m = n * per<T>();
        char *raw = static_cast<char *>(::operator new(HEADER + m * sizeof(E), std::nothrow));
        if (!raw) return nullptr;
        E *p = ((E *)(raw + HEADER));
        count(p) = m;
        for (size_t i = 0; i < m; ++i) new (p + i) E();
        return ((T *)(p));
    }
}

template <class T> void destroy(T *q)
{
    if constexpr (plain<T>::value) {
        free((void *)q);
    } else {
        typedef typename std::remove_cv<elem<T>>::type E;
        E *p = ((E *)(const_cast<typename std::remove_cv<T>::type *>(q)));
        if (!p) return;
        for (size_t i = count(p); i-- > 0;) p[i].~E();
        ::operator delete(((char *)(p)) - HEADER);
    }
}

template <class T> T *resize(T *q, size_t n)
{
    if constexpr (plain<T>::value) {
        return static_cast<T *>(realloc((void *)q, n * sizeof(T)));
    } else {
        if (!q) return make<T>(n);
        T *r = make<T>(n);
        if (!r) return nullptr;           /* q is left as it was, as with realloc */
        typedef elem<T> E;
        E *p = ((E *)(q)), *s = ((E *)(r));
        size_t keep = std::min(count(p), n * per<T>());
        for (size_t i = 0; i < keep; ++i) s[i] = std::move(p[i]);
        destroy(q);
        return r;
    }
}

template <class T> T *set(T *q, int c, size_t bytes)
{
    if constexpr (plain<T>::value) {
        return static_cast<T *>(memset((void *)q, c, bytes));
    } else {
        typedef elem<T> E;
        E *p = ((E *)(q));
        size_t n = bytes / sizeof(E);
        if (c == 0) {
            for (size_t i = 0; i < n; ++i) p[i] = E();
        } else if constexpr (std::is_constructible<E, int>::value) {
            /* memset(p, -1, ...) is the usual way to fill with -1 */
            if ((unsigned char)c != 0xff) abort();
            for (size_t i = 0; i < n; ++i) p[i] = E(-1);
        } else {
            abort();
        }
        return q;
    }
}

template <class T, class S> T *copy(T *d, S *s, size_t bytes)
{
    if constexpr (plain<T>::value && plain<S>::value) {
        return static_cast<T *>(memcpy((void *)d, (const void *)s, bytes));
    } else {
        elem<T> *p = ((elem<T> *)(d));
        const elem<S> *q = ((const elem<S> *)(s));
        for (size_t i = 0, n = bytes / sizeof(elem<T>); i < n; ++i) p[i] = q[i];
        return d;
    }
}

template <class T, class S> T *move(T *d, S *s, size_t bytes)
{
    if constexpr (plain<T>::value && plain<S>::value) {
        return static_cast<T *>(memmove((void *)d, (const void *)s, bytes));
    } else {
        elem<T> *p = ((elem<T> *)(d));
        const elem<S> *q = ((const elem<S> *)(s));
        size_t n = bytes / sizeof(elem<T>);
        if ((const void *)p < (const void *)q)
            for (size_t i = 0; i < n; ++i) p[i] = q[i];
        else
            for (size_t i = n; i-- > 0;) p[i] = q[i];
        return d;
    }
}

template <class T, class S> int cmp(T *a, S *b, size_t bytes)
{
    if constexpr (plain<T>::value && plain<S>::value) {
        return memcmp((const void *)a, (const void *)b, bytes);
    } else {
        const elem<T> *p = ((const elem<T> *)(a));
        const elem<S> *q = ((const elem<S> *)(b));
        for (size_t i = 0, n = bytes / sizeof(elem<T>); i < n; ++i)
            if (!(p[i] == q[i])) return p[i] < q[i] ? -1 : 1;
        return 0;
    }
}

template <class T, class F> void sort(T *b, size_t n, size_t size, F f)
{
    if constexpr (plain<T>::value) {
        qsort((void *)b, n, size, f);
    } else {
        /* sort an index permutation, then move the elements (which may be
         * arrays, so are copied as their innermost elements) into place */
        (void)size;
        std::vector<size_t> order(n);
        for (size_t i = 0; i < n; ++i) order[i] = i;
        std::stable_sort(order.begin(), order.end(),    /* stable, as glibc qsort is */
                  [b, f](size_t x, size_t y) { return f(b + x, b + y) < 0; });
        typedef elem<T> E;
        const size_t k = per<T>();
        std::vector<E> tmp(n * k);
        E *flat = ((E *)(b));
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < k; ++j) tmp[i * k + j] = std::move(flat[order[i] * k + j]);
        for (size_t i = 0; i < n * k; ++i) flat[i] = std::move(tmp[i]);
    }
}

}  // namespace oeisc_mem

#define mem_new(T, n) oeisc_mem::make<T>((size_t)(n))
#define mem_resize(T, p, n) oeisc_mem::resize<T>((p), (size_t)(n))
#define mem_free(p) oeisc_mem::destroy(p)
#define mem_set(p, c, n) oeisc_mem::set((p), (c), (n))
#define mem_copy(d, s, n) oeisc_mem::copy((d), (s), (n))
#define mem_move(d, s, n) oeisc_mem::move((d), (s), (n))
#define mem_cmp(a, b, n) oeisc_mem::cmp((a), (b), (n))
#define mem_qsort(b, n, s, f) oeisc_mem::sort((b), (n), (s), (f))

#endif
#endif
