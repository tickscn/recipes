#ifndef DAINA_NONCOPYABLE_H
#define DAINA_NONCOPYABLE_H
namespace daina {

class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable()  = default;
    ~noncopyable() = default;
};

}  // namespace daina

#endif  // DAINA_NONCOPYABLE_H
